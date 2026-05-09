#ifndef SERVER_H
#define SERVER_H

#include <arpa/inet.h>

#define MAX_USER_LEN 256
#define MAX_MESSAGE_LEN 256
#define MAX_FILENAME_LEN 256
#define MAX_PORT_STR 6
#define MAX_OP_LEN 32
#define BACKLOG 16

#define CODE_OK 0
#define CODE_USER_ERROR 1
#define CODE_ERROR 2
#define CODE_OTHER_ERROR 3

typedef enum {
    USER_DISCONNECTED = 0,
    USER_CONNECTED = 1
} user_state_t;

typedef struct pending_message {
    unsigned int id;
    int has_attachment;
    char sender[MAX_USER_LEN];
    char receiver[MAX_USER_LEN];
    char text[MAX_MESSAGE_LEN];
    char filename[MAX_FILENAME_LEN];
    struct pending_message *next;
} pending_message_t;

typedef struct user {
    char name[MAX_USER_LEN];
    user_state_t state;
    char ip[INET_ADDRSTRLEN];
    int port;
    unsigned int last_msg_id;
    pending_message_t *pending_head;
    pending_message_t *pending_tail;
    struct user *next;
} user_t;

typedef struct client_request {
    int fd;
    char ip[INET_ADDRSTRLEN];
} client_request_t;

#endif
