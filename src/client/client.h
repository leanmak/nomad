#pragma once

#ifndef CLIENT_H
#define CLIENT_H

#include <winsock2.h>
#include <mswsock.h>

#include "../server/server.h"

typedef enum operation_type_e {
    OP_ACCEPT,
    OP_SEND,
    OP_RECV
} OperationType;

typedef struct client_context_s {
    OVERLAPPED ov;
    WSABUF wsa_buffer;
    char *buffer;
    DWORD buffer_len;
    DWORD bytes;
    OperationType operation;
    SOCKET socket;
} ClientContext;

/**
 * @brief Accepts a new client connection on the listening socket.
 * 
 * @return An `ClientContext` structure on success.
 * @return `NULL` on failure.
 */
ClientContext *AcceptNewClient(ServerContext *ctx);

#endif