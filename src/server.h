#ifndef SERVER_HEADER_GUARD
#define SERVER_HEADER_GUARD

#include <arpa/inet.h>
#include <sys/socket.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include "clients.h"
#include "router.h"

struct server
{
  int file_descriptor;
  struct sockaddr_in *handle;
  struct router *router;
};

struct server* new_server(int);
void close_server(struct server *);
void server_listen_callback(struct server *, void *);
void server_listen_router(struct server *);
void server_use_router(struct server *, struct router *);

#endif // SERVER_HEADER_GUARD
