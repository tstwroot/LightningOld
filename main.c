#include "src/server.h"
#include "src/router.h"
#include "src/controllers.h"

int main(int argc, char *argv[])
{
  if(argc != 2)
  {
    fprintf(stderr, "Missing port argument.\n");
    exit(EXIT_FAILURE); 
  }

  int port = atoi(argv[1]);

  struct server *server = new_server(port);

  if(server == NULL)
  {
    fprintf(stderr, "Server Create Error.\n");
    exit(EXIT_FAILURE);
  }

  struct router *router = new_router();

  router_new_get(router, "/", &controller_index);
  router_new_get(router, "/home", &controller_home);
  router_new_get(router, "/contact", &controller_contact);
  router_new_get(router, "/about", &controller_about);
  router_new_post(router, "/test", &controller_test);

  server_use_router(server, router); 
  server_listen_router(server);
  close_server(server);
  return(0);
}
