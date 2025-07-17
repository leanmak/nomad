#pragma once

#ifndef SOCKET_SERVER_H
#define SOCKET_SERVER_H

#include <winsock2.h>
#include <stdbool.h>

extern bool init_wsa;
// This number has no significance, it just popped into my mind.
extern const int MAX_CONNECTIONS;

int startup_winsock_api();
SOCKET create_server_socket(int port);
int accept_client_socket(SOCKET server_socket);

#endif