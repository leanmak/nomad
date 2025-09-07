#pragma once

#ifndef UTILS_H
#define UTILS_H

// Number of worker threads created. N*2 connections are sent on startup to ready-up these threads.
#define NUM_WORKER_THREADS 5

// The port the server socket will be bound to.
#define SERVER_SOCKET_PORT 3000

// The maximum amount of pending connections allowed to queue on the server socket.
#define MAX_PENDING_CONNECTIONS SOMAXCONN

/**
 * @brief Initiates the Winsock API.
 * 
 * @return Exits on failure.
 */
void InitWSA();

/**
 * @brief Allocates an abitrary amount of memory.
 * 
 * @return A void pointer to the allocated memory on success.
 * @return Exits on failure.
 */
void *xmalloc(size_t bytes);

#endif