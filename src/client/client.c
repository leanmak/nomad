#include <stdio.h>
#include <stdbool.h>

#include "client.h"
#include "client_request.h"
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
    
    // for other events (recv, send)
    if(!CreateIoCompletionPort((HANDLE)client_socket, ctx->iocp_handle, (u_long)0, 0)) {
        printf("Failed to bind client socket to IOCP with an error code of: %lu\n", GetLastError());
        
        closesocket(client_socket);
        free(client_ctx);

        return NULL;
    }

    DWORD bytes = 0;
    if(ctx->lpfn_accept_ex(
        ctx->server_socket,
        client_socket,
        client_ctx->buffer, 0, // we don't need a receive operation when accepting the connection so 0 for dwReceiveDataLength
        sizeof(SOCKADDR_IN) + 16, sizeof(SOCKADDR_IN) + 16,
        &bytes,
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

    return client_ctx;
}

int ReceiveDataFromClient(SOCKET client) {
    ClientContext *recv_ctx = malloc(sizeof(ClientContext));
    if(!recv_ctx) {
        printf("Failed to allocate memory for client receive context.\n");

        return -1;
    }

    ZeroMemory(&recv_ctx->ov, sizeof(recv_ctx->ov));

    recv_ctx->buffer_len = CLIENT_RECEIVE_BUFFER;
    recv_ctx->buffer = malloc(CLIENT_RECEIVE_BUFFER);
    if(!recv_ctx->buffer) {
        printf("Failed to allocate memory for recieve context buffer.");
        free(recv_ctx);

        return -1;
    }

    recv_ctx->wsa_buffer.buf = recv_ctx->buffer;
    recv_ctx->wsa_buffer.len = CLIENT_RECEIVE_BUFFER;
    recv_ctx->operation = OP_RECV;
    recv_ctx->socket = client;

    DWORD flags = 0, bytes = 0;

    if(WSARecv(
        client,
        &recv_ctx->wsa_buffer,
        1,
        &bytes, &flags,
        &recv_ctx->ov,
        NULL
    ) == SOCKET_ERROR) {
        int err = WSAGetLastError();

        if(err != ERROR_IO_PENDING) {
            printf("WSARecv() failed with an error code of: %d\n", err);

            free(recv_ctx->buffer);
            free(recv_ctx);

            return -1;
        }
    }

    return 0;
}

int SendDataToClient(SOCKET client, char *request_buffer, ServerContext *ctx) {
    ClientContext *send_ctx = malloc(sizeof(ClientContext));
    if(!send_ctx) {
        printf("Failed to allocate memory for a client's send context.\n");
        free(request_buffer);

        return -1;
    }

    ZeroMemory(&send_ctx->ov, sizeof(send_ctx->ov));

    HTTPRequest *req = ParseHTTPRequest(request_buffer);
    if(!req) {
        free(send_ctx);
        free(request_buffer);

        return -1;
    }

    // test response for now
    char *response;
    if(strcmp(req->status->route, "/") == 0) {
        response = "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\nContent-Length: 9\r\n\r\nHome Page";
    } else {
        response = "HTTP/1.1 404 Not Found\r\nContent-Type: text/plain\r\nContent-Length: 10\r\n\r\nNot a Page";
    }
    
    size_t response_size = strlen(response);

    send_ctx->buffer_len = response_size;
    send_ctx->buffer = malloc(response_size+1);
    if(!send_ctx->buffer) {
        printf("Failed to allocate memory for send context buffer.\n");
        free(request_buffer);
        free(send_ctx);

        return -1;
    }
    strcpy(send_ctx->buffer, response);

    send_ctx->wsa_buffer.buf = send_ctx->buffer;
    send_ctx->wsa_buffer.len = send_ctx->buffer_len;
    send_ctx->operation = OP_SEND;
    send_ctx->socket = client;

    if(WSASend(
        client,
        &send_ctx->wsa_buffer,
        1,
        NULL,
        0,
        &send_ctx->ov,
        NULL
    ) == SOCKET_ERROR) {
        int err = WSAGetLastError();

        if(err != ERROR_IO_PENDING) {
            printf("WSASend() failed with an error code of: %d\n", err);

            free(request_buffer);
            free(send_ctx->buffer);
            free(send_ctx);

            return -1;
        }
    }

    free(request_buffer);
    FreeHTTPRequest(req);

    return 0;
}