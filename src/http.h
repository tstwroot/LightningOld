#ifndef HTTP_HEADER_GUARD
#define HTTP_HEADER_GUARD

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>
#include <sys/stat.h>

#define REQUEST_SIZE 4096

enum HTTP_METHODS
{
  NONE,
  GET,
  POST
};

enum HTTP_METHODS request_method(char *);
char *get_route(char *);
char *send_file(char *);

#endif // HTTP_HEADER_GUARD
