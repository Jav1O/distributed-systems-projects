#include "server.h"

#include "log_rpc_client.h"

#include <errno.h>
#include <netinet/in.h>
#include <pthread.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

static user_t *users = NULL;
static pthread_mutex_t users_mutex = PTHREAD_MUTEX_INITIALIZER;
static volatile sig_atomic_t listen_fd = -1;

static int send_all(int fd, const void *buffer, size_t length)
{
    const char *data = (const char *)buffer;
    size_t sent = 0;

    while (sent < length) {
        ssize_t n = send(fd, data + sent, length - sent, 0);
        if (n <= 0) {
            return -1;
        }
        sent += (size_t)n;
    }

    return 0;
}

static int send_code(int fd, unsigned char code)
{
    return send_all(fd, &code, sizeof(code));
}

static int send_string(int fd, const char *value)
{
    return send_all(fd, value, strlen(value) + 1);
}

static int recv_string(int fd, char *buffer, size_t max_len)
{
    size_t pos = 0;

    if (max_len == 0) {
        return -1;
    }

    while (pos < max_len) {
        char c;
        ssize_t n = recv(fd, &c, sizeof(c), 0);
        if (n <= 0) {
            return -1;
        }
        buffer[pos++] = c;
        if (c == '\0') {
            return 0;
        }
    }

    buffer[max_len - 1] = '\0';
    return -1;
}

static user_t *find_user(const char *name)
{
    user_t *current = users;

    while (current != NULL) {
        if (strcmp(current->name, name) == 0) {
            return current;
        }
        current = current->next;
    }

    return NULL;
}

static void free_pending_messages(pending_message_t *message)
{
    while (message != NULL) {
        pending_message_t *next = message->next;
        free(message);
        message = next;
    }
}

static void remove_user_node(user_t *target)
{
    user_t **current = &users;

    while (*current != NULL) {
        if (*current == target) {
            *current = target->next;
            free_pending_messages(target->pending_head);
            free(target);
            return;
        }
        current = &((*current)->next);
    }
}

static int enqueue_message(user_t *receiver, unsigned int id, const char *sender, const char *text,
                           int has_attachment, const char *filename)
{
    pending_message_t *message = calloc(1, sizeof(*message));
    if (message == NULL) {
        return -1;
    }

    message->id = id;
    message->has_attachment = has_attachment;
    snprintf(message->sender, sizeof(message->sender), "%s", sender);
    snprintf(message->receiver, sizeof(message->receiver), "%s", receiver->name);
    snprintf(message->text, sizeof(message->text), "%s", text);
    if (filename != NULL) {
        snprintf(message->filename, sizeof(message->filename), "%s", filename);
    }

    if (receiver->pending_tail == NULL) {
        receiver->pending_head = message;
        receiver->pending_tail = message;
    } else {
        receiver->pending_tail->next = message;
        receiver->pending_tail = message;
    }

    return 0;
}

static int copy_first_pending(const char *receiver_name, pending_message_t *out)
{
    user_t *receiver = find_user(receiver_name);

    if (receiver == NULL || receiver->pending_head == NULL || receiver->state != USER_CONNECTED) {
        return 0;
    }

    *out = *(receiver->pending_head);
    out->next = NULL;
    return 1;
}

static int remove_pending(const char *receiver_name, const char *sender, unsigned int id)
{
    user_t *receiver = find_user(receiver_name);
    pending_message_t **current;

    if (receiver == NULL) {
        return 0;
    }

    current = &(receiver->pending_head);
    while (*current != NULL) {
        pending_message_t *message = *current;
        if (message->id == id && strcmp(message->sender, sender) == 0) {
            *current = message->next;
            if (receiver->pending_tail == message) {
                receiver->pending_tail = NULL;
                if (receiver->pending_head != NULL) {
                    pending_message_t *tail = receiver->pending_head;
                    while (tail->next != NULL) {
                        tail = tail->next;
                    }
                    receiver->pending_tail = tail;
                }
            }
            free(message);
            return 1;
        }
        current = &(message->next);
    }

    return 0;
}

static void mark_disconnected(const char *name)
{
    user_t *user = find_user(name);

    if (user != NULL) {
        user->state = USER_DISCONNECTED;
        user->ip[0] = '\0';
        user->port = 0;
    }
}

static int open_connection(const char *ip, int port)
{
    int fd;
    struct sockaddr_in addr;

    fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd < 0) {
        return -1;
    }

    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons((uint16_t)port);
    if (inet_pton(AF_INET, ip, &addr.sin_addr) <= 0) {
        close(fd);
        return -1;
    }

    if (connect(fd, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        close(fd);
        return -1;
    }

    return fd;
}

static int deliver_message_to_client(const char *ip, int port, const pending_message_t *message)
{
    char id_text[32];
    int fd = open_connection(ip, port);

    if (fd < 0) {
        return -1;
    }

    snprintf(id_text, sizeof(id_text), "%u", message->id);
    if (send_string(fd, message->has_attachment ? "SEND MESSAGE ATTACH" : "SEND MESSAGE") < 0 ||
        send_string(fd, message->sender) < 0 ||
        send_string(fd, id_text) < 0 ||
        send_string(fd, message->text) < 0 ||
        (message->has_attachment && send_string(fd, message->filename) < 0)) {
        close(fd);
        return -1;
    }

    close(fd);
    return 0;
}

static void notify_sender_ack(const char *sender_name, unsigned int id, int has_attachment,
                              const char *filename)
{
    char ip[INET_ADDRSTRLEN];
    int port = 0;
    int connected = 0;
    int fd;
    char id_text[32];

    pthread_mutex_lock(&users_mutex);
    user_t *sender = find_user(sender_name);
    if (sender != NULL && sender->state == USER_CONNECTED) {
        snprintf(ip, sizeof(ip), "%s", sender->ip);
        port = sender->port;
        connected = 1;
    }
    pthread_mutex_unlock(&users_mutex);

    if (!connected) {
        return;
    }

    fd = open_connection(ip, port);
    if (fd < 0) {
        pthread_mutex_lock(&users_mutex);
        mark_disconnected(sender_name);
        pthread_mutex_unlock(&users_mutex);
        return;
    }

    snprintf(id_text, sizeof(id_text), "%u", id);
    if (send_string(fd, has_attachment ? "SEND MESS ATTACH ACK" : "SEND MESS ACK") < 0 ||
        send_string(fd, id_text) < 0 ||
        (has_attachment && send_string(fd, filename) < 0)) {
        pthread_mutex_lock(&users_mutex);
        mark_disconnected(sender_name);
        pthread_mutex_unlock(&users_mutex);
    }

    close(fd);
}

static int copy_user_endpoint(const char *name, char *ip, size_t ip_len, int *port)
{
    user_t *user = find_user(name);

    if (user == NULL || user->state != USER_CONNECTED) {
        return 0;
    }

    snprintf(ip, ip_len, "%s", user->ip);
    *port = user->port;
    return 1;
}

static void try_deliver_pending(const char *receiver_name)
{
    while (1) {
        pending_message_t message;
        char ip[INET_ADDRSTRLEN];
        int port = 0;
        int has_message;
        int has_endpoint;

        pthread_mutex_lock(&users_mutex);
        has_message = copy_first_pending(receiver_name, &message);
        has_endpoint = copy_user_endpoint(receiver_name, ip, sizeof(ip), &port);
        pthread_mutex_unlock(&users_mutex);

        if (!has_message || !has_endpoint) {
            return;
        }

        if (deliver_message_to_client(ip, port, &message) == 0) {
            pthread_mutex_lock(&users_mutex);
            remove_pending(receiver_name, message.sender, message.id);
            pthread_mutex_unlock(&users_mutex);
            printf("s> SEND MESSAGE %u FROM %s TO %s\n", message.id, message.sender, receiver_name);
            fflush(stdout);
            notify_sender_ack(message.sender, message.id, message.has_attachment, message.filename);
        } else {
            pthread_mutex_lock(&users_mutex);
            mark_disconnected(receiver_name);
            pthread_mutex_unlock(&users_mutex);
            return;
        }
    }
}

static void handle_register(int fd)
{
    char name[MAX_USER_LEN];
    unsigned char code = CODE_ERROR;

    if (recv_string(fd, name, sizeof(name)) == 0) {
        log_rpc_operation(name, "REGISTER", NULL);
        pthread_mutex_lock(&users_mutex);
        if (find_user(name) != NULL) {
            code = CODE_USER_ERROR;
        } else {
            user_t *new_user = calloc(1, sizeof(*new_user));
            if (new_user != NULL) {
                snprintf(new_user->name, sizeof(new_user->name), "%s", name);
                new_user->state = USER_DISCONNECTED;
                new_user->last_msg_id = 0;
                new_user->next = users;
                users = new_user;
                code = CODE_OK;
            }
        }
        pthread_mutex_unlock(&users_mutex);
    } else {
        snprintf(name, sizeof(name), "<unknown>");
    }

    send_code(fd, code);
    printf("s> REGISTER %s %s\n", name, code == CODE_OK ? "OK" : "FAIL");
    fflush(stdout);
}

static void handle_unregister(int fd)
{
    char name[MAX_USER_LEN];
    unsigned char code = CODE_ERROR;

    if (recv_string(fd, name, sizeof(name)) == 0) {
        log_rpc_operation(name, "UNREGISTER", NULL);
        pthread_mutex_lock(&users_mutex);
        user_t *user = find_user(name);
        if (user == NULL) {
            code = CODE_USER_ERROR;
        } else {
            remove_user_node(user);
            code = CODE_OK;
        }
        pthread_mutex_unlock(&users_mutex);
    } else {
        snprintf(name, sizeof(name), "<unknown>");
    }

    send_code(fd, code);
    printf("s> UNREGISTER %s %s\n", name, code == CODE_OK ? "OK" : "FAIL");
    fflush(stdout);
}

static void handle_connect(int fd, const char *client_ip)
{
    char name[MAX_USER_LEN];
    char port_text[MAX_PORT_STR];
    unsigned char code = CODE_OTHER_ERROR;

    if (recv_string(fd, name, sizeof(name)) == 0 &&
        recv_string(fd, port_text, sizeof(port_text)) == 0) {
        char *endptr = NULL;
        long port = strtol(port_text, &endptr, 10);

        log_rpc_operation(name, "CONNECT", NULL);
        if (endptr == port_text || *endptr != '\0' || port < 1 || port > 65535) {
            code = CODE_OTHER_ERROR;
        } else {
            pthread_mutex_lock(&users_mutex);
            user_t *user = find_user(name);
            if (user == NULL) {
                code = CODE_USER_ERROR;
            } else if (user->state == USER_CONNECTED) {
                code = CODE_ERROR;
            } else {
                snprintf(user->ip, sizeof(user->ip), "%s", client_ip);
                user->port = (int)port;
                user->state = USER_CONNECTED;
                code = CODE_OK;
            }
            pthread_mutex_unlock(&users_mutex);
        }
    } else {
        snprintf(name, sizeof(name), "<unknown>");
    }

    send_code(fd, code);
    printf("s> CONNECT %s %s\n", name, code == CODE_OK ? "OK" : "FAIL");
    fflush(stdout);

    if (code == CODE_OK) {
        try_deliver_pending(name);
    }
}

static void handle_disconnect(int fd, const char *client_ip)
{
    char name[MAX_USER_LEN];
    unsigned char code = CODE_OTHER_ERROR;

    if (recv_string(fd, name, sizeof(name)) == 0) {
        log_rpc_operation(name, "DISCONNECT", NULL);
        pthread_mutex_lock(&users_mutex);
        user_t *user = find_user(name);
        if (user == NULL) {
            code = CODE_USER_ERROR;
        } else if (user->state != USER_CONNECTED) {
            code = CODE_ERROR;
        } else if (strcmp(user->ip, client_ip) != 0) {
            code = CODE_OTHER_ERROR;
        } else {
            user->state = USER_DISCONNECTED;
            user->ip[0] = '\0';
            user->port = 0;
            code = CODE_OK;
        }
        pthread_mutex_unlock(&users_mutex);
    } else {
        snprintf(name, sizeof(name), "<unknown>");
    }

    send_code(fd, code);
    printf("s> DISCONNECT %s %s\n", name, code == CODE_OK ? "OK" : "FAIL");
    fflush(stdout);
}

static void handle_users(int fd)
{
    char name[MAX_USER_LEN];
    unsigned char code = CODE_ERROR;
    char **names = NULL;
    size_t count = 0;

    if (recv_string(fd, name, sizeof(name)) == 0) {
        log_rpc_operation(name, "USERS", NULL);
        pthread_mutex_lock(&users_mutex);
        user_t *requester = find_user(name);
        if (requester == NULL) {
            code = CODE_ERROR;
        } else if (requester->state != USER_CONNECTED) {
            code = CODE_USER_ERROR;
        } else {
            user_t *current;
            for (current = users; current != NULL; current = current->next) {
                if (current->state == USER_CONNECTED) {
                    count++;
                }
            }

            names = calloc(count == 0 ? 1 : count, sizeof(*names));
            if (names == NULL) {
                code = CODE_ERROR;
            } else {
                size_t i = 0;
                for (current = users; current != NULL; current = current->next) {
                    if (current->state == USER_CONNECTED) {
                        char entry[MAX_USER_LEN + INET_ADDRSTRLEN + MAX_PORT_STR + 8];
                        snprintf(entry, sizeof(entry), "%s :: %s :: %d",
                                 current->name, current->ip, current->port);
                        names[i] = strdup(entry);
                        if (names[i] == NULL) {
                            code = CODE_ERROR;
                            break;
                        }
                        i++;
                    }
                }
                if (i == count) {
                    code = CODE_OK;
                }
            }
        }
        pthread_mutex_unlock(&users_mutex);
    } else {
        snprintf(name, sizeof(name), "<unknown>");
    }

    if (send_code(fd, code) == 0 && code == CODE_OK) {
        char count_text[32];
        snprintf(count_text, sizeof(count_text), "%zu", count);
        if (send_string(fd, count_text) == 0) {
            for (size_t i = 0; i < count; i++) {
                if (send_string(fd, names[i]) < 0) {
                    break;
                }
            }
        }
    }

    if (names != NULL) {
        for (size_t i = 0; i < count; i++) {
            free(names[i]);
        }
        free(names);
    }

    printf("s> CONNECTEDUSERS %s\n", code == CODE_OK ? "OK" : "FAIL");
    fflush(stdout);
}

static void handle_send_common(int fd, int has_attachment)
{
    char sender_name[MAX_USER_LEN];
    char receiver_name[MAX_USER_LEN];
    char text[MAX_MESSAGE_LEN];
    char filename[MAX_FILENAME_LEN];
    unsigned char code = CODE_ERROR;
    unsigned int id = 0;
    int receiver_connected = 0;
    char receiver_ip[INET_ADDRSTRLEN];
    int receiver_port = 0;

    filename[0] = '\0';

    if (recv_string(fd, sender_name, sizeof(sender_name)) == 0 &&
        recv_string(fd, receiver_name, sizeof(receiver_name)) == 0 &&
        recv_string(fd, text, sizeof(text)) == 0 &&
        (!has_attachment || recv_string(fd, filename, sizeof(filename)) == 0)) {
        log_rpc_operation(sender_name, has_attachment ? "SENDATTACH" : "SEND",
                          has_attachment ? filename : NULL);
        pthread_mutex_lock(&users_mutex);
        user_t *sender = find_user(sender_name);
        user_t *receiver = find_user(receiver_name);
        if (sender == NULL || receiver == NULL) {
            code = CODE_USER_ERROR;
        } else {
            sender->last_msg_id++;
            if (sender->last_msg_id == 0) {
                sender->last_msg_id = 1;
            }
            id = sender->last_msg_id;
            if (enqueue_message(receiver, id, sender_name, text, has_attachment, filename) == 0) {
                code = CODE_OK;
                if (receiver->state == USER_CONNECTED) {
                    receiver_connected = 1;
                    snprintf(receiver_ip, sizeof(receiver_ip), "%s", receiver->ip);
                    receiver_port = receiver->port;
                }
            }
        }
        pthread_mutex_unlock(&users_mutex);
    } else {
        snprintf(sender_name, sizeof(sender_name), "<unknown>");
        snprintf(receiver_name, sizeof(receiver_name), "<unknown>");
    }

    if (send_code(fd, code) == 0 && code == CODE_OK) {
        char id_text[32];
        snprintf(id_text, sizeof(id_text), "%u", id);
        send_string(fd, id_text);
    }

    if (code == CODE_OK) {
        if (receiver_connected) {
            pending_message_t message;
            message.id = id;
            message.has_attachment = has_attachment;
            snprintf(message.sender, sizeof(message.sender), "%s", sender_name);
            snprintf(message.receiver, sizeof(message.receiver), "%s", receiver_name);
            snprintf(message.text, sizeof(message.text), "%s", text);
            snprintf(message.filename, sizeof(message.filename), "%s", filename);
            message.next = NULL;

            if (deliver_message_to_client(receiver_ip, receiver_port, &message) == 0) {
                pthread_mutex_lock(&users_mutex);
                remove_pending(receiver_name, sender_name, id);
                pthread_mutex_unlock(&users_mutex);
                printf("s> SEND MESSAGE %u FROM %s TO %s\n", id, sender_name, receiver_name);
                fflush(stdout);
                notify_sender_ack(sender_name, id, has_attachment, filename);
            } else {
                pthread_mutex_lock(&users_mutex);
                mark_disconnected(receiver_name);
                pthread_mutex_unlock(&users_mutex);
            }
        } else {
            printf("s> MESSAGE %u FROM %s TO %s STORED\n", id, sender_name, receiver_name);
            fflush(stdout);
        }
    }
}

static void handle_send(int fd)
{
    handle_send_common(fd, 0);
}

static void handle_sendattach(int fd)
{
    handle_send_common(fd, 1);
}

static void *handle_client(void *arg)
{
    client_request_t *request = (client_request_t *)arg;
    int fd = request->fd;
    char client_ip[INET_ADDRSTRLEN];
    char operation[MAX_OP_LEN];

    snprintf(client_ip, sizeof(client_ip), "%s", request->ip);
    free(request);

    if (recv_string(fd, operation, sizeof(operation)) == 0) {
        if (strcmp(operation, "REGISTER") == 0) {
            handle_register(fd);
        } else if (strcmp(operation, "UNREGISTER") == 0) {
            handle_unregister(fd);
        } else if (strcmp(operation, "CONNECT") == 0) {
            handle_connect(fd, client_ip);
        } else if (strcmp(operation, "DISCONNECT") == 0) {
            handle_disconnect(fd, client_ip);
        } else if (strcmp(operation, "USERS") == 0) {
            handle_users(fd);
        } else if (strcmp(operation, "SEND") == 0) {
            handle_send(fd);
        } else if (strcmp(operation, "SENDATTACH") == 0) {
            handle_sendattach(fd);
        } else {
            send_code(fd, CODE_ERROR);
        }
    }

    close(fd);
    return NULL;
}

static int parse_port(int argc, char **argv)
{
    if (argc != 3 || strcmp(argv[1], "-p") != 0) {
        return -1;
    }

    char *endptr = NULL;
    long port = strtol(argv[2], &endptr, 10);
    if (endptr == argv[2] || *endptr != '\0' || port < 1024 || port > 65535) {
        return -1;
    }

    return (int)port;
}

static void handle_sigint(int signo)
{
    (void)signo;
    if (listen_fd >= 0) {
        close((int)listen_fd);
        listen_fd = -1;
    }
}

int main(int argc, char **argv)
{
    int port = parse_port(argc, argv);
    int server_fd;
    int opt = 1;
    struct sockaddr_in addr;

    if (port < 0) {
        fprintf(stderr, "Usage: ./server -p <port>\n");
        return EXIT_FAILURE;
    }

    signal(SIGINT, handle_sigint);

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) {
        perror("socket");
        return EXIT_FAILURE;
    }

    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        perror("setsockopt");
        close(server_fd);
        return EXIT_FAILURE;
    }

    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons((uint16_t)port);

    if (bind(server_fd, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        perror("bind");
        close(server_fd);
        return EXIT_FAILURE;
    }

    if (listen(server_fd, BACKLOG) < 0) {
        perror("listen");
        close(server_fd);
        return EXIT_FAILURE;
    }

    listen_fd = server_fd;
    printf("s> init server 0.0.0.0:%d\n", port);
    printf("s>\n");
    fflush(stdout);

    while (1) {
        struct sockaddr_in client_addr;
        socklen_t client_len = sizeof(client_addr);
        int client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &client_len);
        client_request_t *request;
        pthread_t thread;

        if (client_fd < 0) {
            if (errno == EINTR || listen_fd < 0) {
                break;
            }
            continue;
        }

        request = calloc(1, sizeof(*request));
        if (request == NULL) {
            close(client_fd);
            continue;
        }

        request->fd = client_fd;
        if (inet_ntop(AF_INET, &client_addr.sin_addr, request->ip, sizeof(request->ip)) == NULL) {
            snprintf(request->ip, sizeof(request->ip), "0.0.0.0");
        }

        if (pthread_create(&thread, NULL, handle_client, request) != 0) {
            close(client_fd);
            free(request);
            continue;
        }
        pthread_detach(thread);
    }

    close(server_fd);
    return EXIT_SUCCESS;
}
