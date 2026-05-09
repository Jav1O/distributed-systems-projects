#include "log_rpc_client.h"

#include "log_rpc.h"

#include <stdlib.h>
#include <string.h>
#include <sys/time.h>

int log_rpc_operation(const char *user, const char *operation, const char *filename)
{
    char *host = getenv("LOG_RPC_IP");
    CLIENT *client_handle;
    log_request request;
    int *result;
    struct timeval timeout;

    if (host == NULL || host[0] == '\0') {
        return 0;
    }

    client_handle = clnt_create(host, LOG_RPC_PROG, LOG_RPC_VERS, "udp");
    if (client_handle == NULL) {
        return -1;
    }

    timeout.tv_sec = 1;
    timeout.tv_usec = 0;
    clnt_control(client_handle, CLSET_TIMEOUT, (char *)&timeout);

    request.user = (char *)(user != NULL ? user : "");
    request.operation = (char *)(operation != NULL ? operation : "");
    request.filename = (char *)(filename != NULL ? filename : "");

    result = log_operation_1(&request, client_handle);
    clnt_destroy(client_handle);

    if (result == NULL) {
        return -1;
    }

    return *result;
}
