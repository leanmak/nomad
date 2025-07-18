#pragma once

#ifndef CLIENT_HANDLER_H
#define CLIENT_HANDLER_H

#include <winsock2.h>

int handle_connection(SOCKET client_socket);
char* read_file_to_string(char *file_name);

#endif