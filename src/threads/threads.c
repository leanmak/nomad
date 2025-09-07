#include <stdio.h>
#include <stdbool.h>

#include "threads.h"
#include "../client/client.h"
#include "../utils.h"

int SpawnWorkerThreads(ServerContext *ctx) {
    for(int i = 0; i < NUM_WORKER_THREADS; i++) {
        HANDLE h = CreateThread(NULL, 0, WorkerThread, (LPVOID)ctx, 0, NULL);

        if(h == NULL) {
            printf("Failed to create worker thread with an error code of: %lu\n", GetLastError());
            return -1;
        }

        // The worker threads are detached, so we don't need to keep track of the thread handles.
        if(!CloseHandle(h)) {
            printf("Failed to close worker thread handle with an error code of: %lu\n", GetLastError());
            return -1;
        }
    }

    return 0;
}

DWORD WINAPI WorkerThread(LPVOID param) {
    ServerContext *ctx = (ServerContext*)param;
    
    printf("New worker thread active...\n");

    while(true) {
        DWORD bytes = 0;
        ULONG_PTR completion_key = 0;
        LPOVERLAPPED ov = NULL;

        BOOL ok = GetQueuedCompletionStatus(ctx->iocp_handle, &bytes, &completion_key, &ov, INFINITE);

        if (!ov) {
            // IOCP shutdown signal (if you ever PostQueuedCompletionStatus with NULL ov)
            if (!ok && GetLastError() == WAIT_TIMEOUT) continue;
            break;
        }

        if(!ok) {
            printf("GetQueuedCompletionStatus() failed with an error code of: %lu\n", GetLastError());
            continue;
        }

        ClientContext *client_ctx = CONTAINING_RECORD(ov, ClientContext, ov);
        SOCKET client_socket = client_ctx->socket;

        closesocket(client_socket);
        free(client_ctx);

        AcceptNewClient(ctx);
    }

    return 0;
}