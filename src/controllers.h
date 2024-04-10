#ifndef CONTROLLERS_HEADER_GUARD
#define CONTROLLERS_HEADER_GUARD

#include "http.h" 
#include <sys/socket.h>

void controller_index(int file_descriptor);
void controller_home(int file_descriptor);
void controller_contact(int file_descriptor);
void controller_about(int file_descriptor);
void controller_test(int file_descriptor);

#endif // CONTROLLERS_HEADER_GUARD
