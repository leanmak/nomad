#pragma once

#ifndef CLIENT_HANDLER_H
#define CLIENT_HANDLER_H

#include <winsock2.h>

int handle_connection(SOCKET client_socket);

#endif