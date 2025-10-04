#pragma once

#ifndef UTILS_H
#define UTILS_H

// Number of worker threads created. N*2 connections are sent on startup to ready-up these threads.
#define NUM_WORKER_THREADS 5

// The port the server socket will be bound to.
#define SERVER_SOCKET_PORT 3000

// The maximum amount of pending connections allowed to queue on the server socket.
#define MAX_PENDING_CONNECTIONS SOMAXCONN

// The buffer size for the data sent from the client.
#define CLIENT_RECEIVE_BUFFER 2048

// The maximum amount of headers that can be parsed.
#define MAX_HEADER_COUNT 512

#include "client/client.h"
#include "client/client_request.h"

/**
 * @brief Initiates the Winsock API.
 * 
 * @return Exits on failure.
 */
void InitWSA();

/**
 * @brief Duplicates a CRLF-suffixed string and removes the trailing '\r' character.
 * 
 * @return The duplicated string if successful.
 * @return `NULL` if unsuccessful.
 */
char *DuplicateCRLFString(const char *s);

/**
 * @brief Frees an allocated `ClientContext` pointer.
 */
void FreeClientContext(ClientContext *ctx);

typedef struct http_request_s HTTPRequest;

/**
 * @brief Frees an allocated `HTTPRequest` pointer.
 */
void FreeHTTPRequest(HTTPRequest *req);

/**
 * @brief Fetches a file's content.
 * 
 * @return The file's content on success.
 * @return `NULL` on failure.
 */
char *GetFileContent(char *file_path);

#endif