#ifndef ROUTER_HEADER_GUARD
#define ROUTER_HEADER_GUARD

#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <pthread.h>
#include "clients.h"
#include "http.h"

struct router_arg
{
  struct server *server;
  struct client *client;
};

struct route
{
  char *path;
  void (*function)(int);
  struct route *next;
};

struct router
{
  struct route *get;
  struct route *post;
  size_t get_size;
  size_t post_size;
};

void *router(void *args);
struct router *new_router();
struct route *new_route(char *, void (*function)(int));
void add_route(struct route *, char *, void (*function)(int));
void router_new_get(struct router *, char *, void (*function)(int));
void router_new_post(struct router *, char *, void (*function)(int));

#endif // ROUTER_HEADER_GUARD
