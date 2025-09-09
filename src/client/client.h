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

/**
 * @brief Receives data from a new client connection.
 * 
 * @returns A non-zero integer on failure.
 */
int ReceiveDataFromClient(SOCKET client);

/**
 * @brief Sends data to a client connection.
 * 
 * @returns A non-zero integer on failure.
 */
int SendDataToClient(SOCKET client, char *request_buffer, ServerContext *ctx);

#endif