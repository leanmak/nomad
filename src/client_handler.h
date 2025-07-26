#pragma once

#ifndef CLIENT_HANDLER_H
#define CLIENT_HANDLER_H

#include <winsock2.h>

int handle_connection(SOCKET client_socket);
void* handle_connection_test(void* client_socket_v_ptr);
char* read_file_to_string(char *file_name);
int serve_page(char *status_line, char *content_type, char *file_name, SOCKET *client_socket);

#endif