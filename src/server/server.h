#pragma once

#ifndef SERVER_H
#define SERVER_H

#include <winsock2.h>
#include <mswsock.h>

#include "../cache/cache.h"

// Holds information about the current state of the program.
typedef struct server_context_s {
    HANDLE iocp_handle;
    SOCKET server_socket;
    LPFN_ACCEPTEX lpfn_accept_ex;
    FileCache *cache;
} ServerContext;

/**
 * @brief Creates and binds a listening socket on the loopback IP address and the `SERVER_SOCKET_PORT` port in `utils.h`.
 * 
 * @return A valid `SOCKET` handle on success.
 * @return `INVALID_SOCKET` on any operation's failure.
 */
SOCKET CreateServerSocket();

/**
 * @brief Creates the server context struct with the `AcceptEx()` function.
 * 
 * @return A pointer to a `ServerSocket` struct.
 * @return `NULL` on failure.
 */
ServerContext *NewServerContext(SOCKET server_socket, HANDLE iocp_handle);

#endif