#include "router.h"
#include "http.h"
#include "server.h"

struct router *new_router()
{
  struct router *n_router = calloc(1, sizeof(struct router));
  n_router->get = NULL;
  n_router->post = NULL;
  return(n_router);
}

struct route *new_route(char *path, void (*function)(int))
{
  struct route *n_route = calloc(1, sizeof(struct route));
  n_route->path = calloc(strlen(path), sizeof(char));
  strncpy(n_route->path, path, strlen(path));
  n_route->function = function;
  return(n_route);
}

void add_route(struct route *route, char *path, void (*function)(int))
{
  if(route == NULL) 
  {
    route = new_route(path, function);
    return;
  }
  
  if(route->next == NULL)
  {
    route->next = new_route(path, function);
    return;
  }

  struct route *aux = route->next;

  while(aux != NULL)
  {
    aux = aux->next;
  }

  aux = new_route(path, function);
  return;
}

void router_new_get(struct router *router, char *path, void (*function)(int))
{
  if(router->get == NULL)
  {
    router->get = new_route(path, function);
    return;
  }

  struct route *aux = router->get;

  while(aux->next != NULL)
  {
    aux = aux->next;
  }

  aux->next = new_route(path, function);
  router->get_size++;
  return;
}

void router_new_post(struct router *router, char *path, void (*function)(int))
{
  if(router->post == NULL)
  {
    router->post = new_route(path, function);
    return;
  }

  struct route *aux = router->post;

  while(aux->next != NULL)
  {
    aux = aux->next;
  }

  aux->next = new_route(path, function);
  router->post_size++;
  return;
}

void (*get_function(struct route *route, char *path))(int)
{
  struct route *aux = route;

  while(aux != NULL)
  { 
    if(strcmp(aux->path, path) == 0)
    {
      return(aux->function);
    } 
    aux = aux->next;
  }
  return(NULL);
}

void *router(void *args)
{
  struct router_arg *router_arg = (struct router_arg*)args;
  struct client *new_client = router_arg->client; 
  char request[REQUEST_SIZE]; 
  recv(new_client->file_descriptor, request, REQUEST_SIZE, 0); 
  struct route *route;
  enum HTTP_METHODS method = request_method(request);
  void (*function)(int);
  char *default_response = "HTTP/1.1 404 Not Found\nServer: Lighting 0.1 Alpha\nContent-Type: text/html; charset=UTF-8\n\n404 Not Found";

  switch (method)
  {
    case GET:
      route = router_arg->server->router->get;
      break;
    case POST:
      route = router_arg->server->router->post;
      break;
    default:
      close(new_client->file_descriptor);
      remove_client(new_client->main, new_client->thread_id);
      return(NULL);
      break;
  }

  function = get_function(route, get_route(request));

  if(function != NULL)
  { 
    function(new_client->file_descriptor);
  }
  else
  {
    send(new_client->file_descriptor, default_response, strlen(default_response), 0);
  }

  close(new_client->file_descriptor);
  remove_client(new_client->main, new_client->thread_id);
  return(NULL);
}
