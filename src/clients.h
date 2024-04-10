#ifndef CLIENTS_HEADER_GUARD
#define CLIENTS_HEADER_GUARD

#include <pthread.h>
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>

struct client
{
  struct clients *main;
  struct client *next;
  int file_descriptor;
  pthread_t thread_id;
};

struct clients
{
  struct client *start;
  struct client *end;
  size_t size;
  socklen_t client_size;
  pthread_mutex_t lock;
};

struct clients *new_clients();
struct client *new_client(int);
struct client *insert_client(struct clients *, int);
int remove_client(struct clients *, pthread_t);

#endif // CLIENTS_HEADER_GUARD
