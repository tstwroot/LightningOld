#include "clients.h"

struct clients *new_clients()
{
  struct clients *n_clients = calloc(1, sizeof(struct clients));
  n_clients->size = 0;
  n_clients->start = NULL;
  n_clients->client_size = sizeof(struct sockaddr_in);
  return(n_clients);
}

struct client *new_client(int file_descriptor)
{
  struct client *n_client = calloc(1, sizeof(struct client));
  n_client->file_descriptor = file_descriptor;
  n_client->next = NULL;
  n_client->main = NULL;
  return(n_client);
}

struct client *insert_client(struct clients *clients, int file_descriptor)
{
  if(clients->size == 0)
  {
    clients->start = new_client(file_descriptor);

    if(clients->start == NULL)
    {
      return(NULL);
    }
     
    clients->end = clients->start;
    clients->size++;
    return(clients->start);
  }
  
  clients->end->next = new_client(file_descriptor);

  if(clients->end->next == NULL)
  {
    return(NULL);
  }

  clients->end = clients->end->next;
  clients->size++;
  return(clients->end);
}

int remove_client(struct clients *clients, pthread_t thread_id)
{
  if(clients->size == 0)
  {
    return(0);
  }

  if(clients->start->thread_id == thread_id) 
  {
    if(clients->size == 1)
    {
      free(clients->start);
      clients->start = NULL;
      clients->end = NULL;
      clients->size--;
      return(1);
    }

    struct client *be_removed = clients->start;
    clients->start = clients->start->next;

    if(clients->start->next == NULL)
    {
      clients->end = clients->start;
    }

    free(be_removed);
    clients->size--;
    return(1);
  }

  struct client *aux = clients->start;

  while(aux->next != NULL)
  {
    if(aux->next->thread_id == thread_id)
    {
      struct client *be_removed = aux->next;
      if(be_removed->next != NULL)
      {
        aux->next = be_removed->next;
      }
      else
      {
        clients->end = aux;
        clients->end->next = NULL;
      }
      
      free(be_removed);
      clients->size--;
      return(1);
    }

    aux = aux->next;
  } 

  return(0);
}
