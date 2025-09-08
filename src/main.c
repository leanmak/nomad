#include <stdio.h>
#include <stdbool.h>
#include <winsock2.h>
#include <mswsock.h>

#include "utils.h"
#include "server/server.h"
#include "client/client.h"
#include "threads/threads.h"

int main() {
    InitWSA();

    // 0 for concurrent thread count = #. of cores
    HANDLE iocp_handle = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, (ULONG_PTR)0, 0); 
    if(iocp_handle == NULL) {
        printf("\nFailed to create IOCP handle with an error code of: %lu", GetLastError());
        WSACleanup();

        return 1;
    }

    SOCKET server_socket = CreateServerSocket();
    if(server_socket == INVALID_SOCKET) {
        CloseHandle(iocp_handle);
        WSACleanup();

        return 1;
    }

    if(CreateIoCompletionPort((HANDLE)server_socket, iocp_handle, (ULONG_PTR)0, 0) == false) {
        printf("Failed to bind server socket to IOCP with an error code of: %lu\n", GetLastError());
        CloseHandle(iocp_handle);
        closesocket(server_socket);
        WSACleanup();

        exit(EXIT_FAILURE);
    }

    ServerContext *ctx = NewServerContext(server_socket, iocp_handle);
    if(ctx == NULL) {
        CloseHandle(iocp_handle);
        closesocket(server_socket);
        WSACleanup();

        exit(EXIT_FAILURE);
    }

    if(SpawnWorkerThreads(ctx) != 0) {
        CloseHandle(iocp_handle);
        closesocket(server_socket);
        WSACleanup();

        free(ctx);

        exit(EXIT_FAILURE);
    }

    // Spawn an abitrary amount of connections to ready up N threads.
    for(int i = 0; i < NUM_WORKER_THREADS*2; i++) {
        AcceptNewClient(ctx);
    }

    Sleep(INFINITE);
}