#include "log_rpc.h"

#include <stdio.h>

int *log_operation_1_svc(log_request *request, struct svc_req *svc)
{
    static int result;
    (void)svc;

    result = 0;
    if (request == NULL) {
        return &result;
    }

    printf("%s %s", request->user != NULL ? request->user : "",
           request->operation != NULL ? request->operation : "");
    if (request->filename != NULL && request->filename[0] != '\0') {
        printf(" %s", request->filename);
    }
    printf("\n");
    fflush(stdout);

    return &result;
}
