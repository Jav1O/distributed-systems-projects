CC = gcc
CFLAGS = -Wall -Wextra -Wno-unused-variable -Wno-unused-parameter -Wno-cast-function-type -pthread -I/usr/include/tirpc
LDLIBS = -ltirpc

.PHONY: all clean

all: server log_rpc_server

log_rpc.h log_rpc_clnt.c log_rpc_svc.c log_rpc_xdr.c: log_rpc.x
	rpcgen -C log_rpc.x

server: server.c server.h log_rpc_client.c log_rpc_client.h log_rpc.h log_rpc_clnt.c log_rpc_xdr.c
	$(CC) $(CFLAGS) -o server server.c log_rpc_client.c log_rpc_clnt.c log_rpc_xdr.c $(LDLIBS)

log_rpc_server: log_rpc_server.c log_rpc_svc.c log_rpc_xdr.c log_rpc.h
	$(CC) $(CFLAGS) -o log_rpc_server log_rpc_server.c log_rpc_svc.c log_rpc_xdr.c $(LDLIBS)

clean:
	rm -f server log_rpc_server
