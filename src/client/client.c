#include <stdio.h>
#include <stdbool.h>

#include "client.h"
#include "../utils.h"

ClientContext *AcceptNewClient(ServerContext *ctx) {
    SOCKET client_socket = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, WSA_FLAG_OVERLAPPED);
    if(client_socket == INVALID_SOCKET) {
        int err = WSAGetLastError();
        printf("WSASocket() (client) failed with an error code of: %d\n", err);

        return NULL;
    }

    DWORD addr_len = (DWORD)((sizeof(SOCKADDR_STORAGE) + 16) * 2);
    ClientContext *client_ctx = malloc(sizeof(ClientContext));
    if(!client_ctx) {
        printf("Failed to allocate memory for client socket io_context.\n");
        closesocket(client_socket);

        return NULL;
    }

    ZeroMemory(&client_ctx->ov, sizeof(client_ctx->ov));
    
    client_ctx->buffer_len = addr_len;
    client_ctx->buffer = malloc(addr_len);
    if(!client_ctx->buffer) {
        printf("Failed to allocate for recieve context buffer.\n");

        closesocket(client_socket);
        free(client_ctx);

        return NULL;
    }

    client_ctx->operation = OP_ACCEPT;
    client_ctx->socket = client_socket;
    
    if(ctx->lpfn_accept_ex(
        ctx->server_socket,
        client_socket,
        client_ctx->buffer, 0, // we don't need a receive operation when accepting the connection so 0 for dwReceiveDataLength
        sizeof(SOCKADDR_IN) + 16, sizeof(SOCKADDR_IN) + 16,
        &client_ctx->bytes,
        &client_ctx->ov
    ) == false) {
        int err = WSAGetLastError();

        // skip pending errors (we're waiting for connections to come it)
        if(err != ERROR_IO_PENDING) {
            printf("AcceptEx failed with an error code of: %d\n", err);

            closesocket(client_socket);
            free(client_ctx);

            return NULL;
        }
    }

    // for other events (recv, send)
    if(!CreateIoCompletionPort((HANDLE)client_socket, ctx->iocp_handle, (u_long)0, 0)) {
        printf("Failed to bind client socket to IOCP with an error code of: %lu\n", GetLastError());
        
        closesocket(client_socket);
        free(client_ctx);

        return NULL;
    }

    return client_ctx;
}