#include "controllers.h" 

void controller_index(int file_descriptor)
{
  char *response = send_file("public/index.html");
  send(file_descriptor, response, strlen(response), 0);
  return;
}

void controller_home(int file_descriptor)
{
  char *response = send_file("public/home.html");
  send(file_descriptor, response, strlen(response), 0);
  return;
}

void controller_contact(int file_descriptor)
{
  char *response = send_file("public/contact.html");
  send(file_descriptor, response, strlen(response), 0);
  return;
}

void controller_about(int file_descriptor)
{
  char *response = send_file("public/about.html");
  send(file_descriptor, response, strlen(response), 0);
  return;
}

void controller_test(int file_descriptor)
{
  char *response_content = "HTTP/1.1 200 OK\nServer: Lighting 0.1 Alpha\nContent-Type: application/json";
  char *json = "{message: \"ok\"}";
  char *response = calloc(strlen(response_content) + strlen(json) + 1, sizeof(char));
  sprintf(response, "%s\n\n%s", response_content, json);
  send(file_descriptor, response, strlen(response), 0);
  free(response);
  return;
}
