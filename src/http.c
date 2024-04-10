#include "http.h"
#include <stdio.h>

enum HTTP_METHODS request_method(char *request)
{
  regex_t regex;
  regcomp(&regex, "^GET /([^ ]*) HTTP/1\\.1?.+", REG_EXTENDED); 
  if(regexec(&regex, request, 0, NULL, 0) == 0)
  {
    return(GET);
  }

  regcomp(&regex, "^POST /([^ ]*) HTTP/1\\.1?.+", REG_EXTENDED);
  if(regexec(&regex, request, 0, NULL, 0) == 0) 
  {
    return(POST);
  }

  return(NONE);
}

char *get_route(char *request)
{
  if(request_method(request) == NONE)
  {
    return(NULL);
  }

  char *buffer = calloc(REQUEST_SIZE, sizeof(char)); // IMPORTANT: IMPLEMENT IT WITH LINKEDLIST.
  for(int i = 0; request[i] != 0; i++)
  {
    if(request[i] == '/')
    {
      int j = 0;
      while(request[i] != ' ')
      {

        buffer[j] = request[i];
        i++;
        j++;
      }
      break;
    }
  }

  return(buffer);
}

char *send_file(char *filepath)
{
  FILE *file = fopen(filepath, "r");

  if(file == NULL)
  {
    return(NULL);
  }

  fseek(file, 0, SEEK_END);
  size_t size = ftell(file); 
  rewind(file);

  char *response_content = "HTTP/1.1 200 OK\nServer: Lighting 0.1 Alpha\nContent-Type: text/html; charset=UTF-8";
  char *file_content = calloc(size, sizeof(char));
  fread(file_content, sizeof(char), size, file); 
  char *output = calloc(size + strlen(response_content) + 3, sizeof(char));
  sprintf(output, "%s\n\n%s", response_content, file_content);
  free(file_content);
  return(output);
}
