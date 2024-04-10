#include "server.h"
#include "router.h"

struct server *new_server(int port)
{
  struct server *n_server = malloc(sizeof(struct server));
  n_server->handle = malloc(sizeof(struct sockaddr_in));
  
  n_server->file_descriptor = socket(AF_INET, SOCK_STREAM, 0); 
  n_server->handle->sin_family = AF_INET;
  n_server->handle->sin_port = htons(port);
  n_server->handle->sin_addr.s_addr = INADDR_ANY;
  memset(n_server->handle->sin_zero, 0, sizeof(n_server->handle->sin_zero));
  
  int one = 1;
  setsockopt(n_server->file_descriptor, SOL_SOCKET, SO_REUSEADDR, &one, sizeof(int));

  if(bind(n_server->file_descriptor, (const struct sockaddr*)n_server->handle, (socklen_t)sizeof(struct sockaddr_in)) == -1)
  {
    fprintf(stderr, "Bind error.\n");
    close_server(n_server); 
    return(NULL);
  }

  if(listen(n_server->file_descriptor, 10) == -1)
  {
    fprintf(stderr, "Listen error.\n");
    close_server(n_server);
    return(NULL);
  }
  
  n_server->router = NULL;
  return(n_server);
}

void server_use_router(struct server *server, struct router *router)
{
  server->router = router;
  return;
}

void server_listen_router(struct server *server)
{ 
  struct router_arg *router_arg = calloc(1, sizeof(struct router_arg));
  struct clients *clients = new_clients();
  router_arg->server = server;
  int client_fd = 0;
  
  printf("Up on: \e[0;32mhttp://localhost:%d\e[0m\n", htons(server->handle->sin_port));

  while(1)
  {
    if((client_fd = accept(server->file_descriptor, NULL, NULL)) < 0) 
    {
      fprintf(stderr, "Accept Error.\n");
      exit(EXIT_FAILURE);
    }

    struct client *new_client = insert_client(clients, client_fd);

    if(new_client == NULL)
    {
      fprintf(stderr, "New Client Error.\n");
      exit(EXIT_FAILURE);
    }

    new_client->main = clients;
    router_arg->client = new_client; 

    if(pthread_create(&new_client->thread_id, NULL, router, (void*)router_arg) != 0)
    {
      fprintf(stderr, "New Thread Error.\n");
      break;
    }
  }

  struct client *aux = clients->start;

  while(aux != NULL)
  {
    pthread_join(aux->thread_id, NULL);
    aux = aux->next;
  }

  close_server(server);
  exit(EXIT_FAILURE);
}

void server_listen_callback(struct server *server, void *callback)
{
  struct clients *clients = new_clients();
  
  int client_fd = 0;
  
  printf("Up on: \e[0;32mhttp://localhost:%d\e[0m\n", htons(server->handle->sin_port));

  while(1)
  {
    if((client_fd = accept(server->file_descriptor, NULL, NULL)) < 0) 
    {
      fprintf(stderr, "Accept Error.\n");
      exit(EXIT_FAILURE);
    }

    struct client *new_client = insert_client(clients, client_fd);

    if(new_client == NULL)
    {
      fprintf(stderr, "New Client Error.\n");
      exit(EXIT_FAILURE);
    }

    new_client->main = clients;

    if(pthread_create(&new_client->thread_id, NULL, callback, (void*)new_client) != 0)
    {
      fprintf(stderr, "New Thread Error.\n");
      break;
    }
  }

  struct client *aux = clients->start;

  while(aux != NULL)
  {
    pthread_join(aux->thread_id, NULL);
    aux = aux->next;
  }

  close_server(server);
  exit(EXIT_FAILURE);
}

void close_server(struct server *server)
{
  free(server->handle);
  shutdown(server->file_descriptor, SHUT_RDWR);
  close(server->file_descriptor);
  free(server);
  return;
}
