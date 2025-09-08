#pragma once

#ifndef THREADS_H
#define THREADS_H

#include <processthreadsapi.h>

#include "../server/server.h"

/**
 * @brief Create the worker threads that'll continously listen to the IOCP queue.
 * 
 * @return A non-zero value on failure.
 */
int SpawnWorkerThreads(ServerContext *ctx);

/**
 * @brief The worker thread looping function. These threads listen to the IOCP queue.
 */
DWORD WINAPI WorkerThread(LPVOID param);

#endif