#include <stdio.h>
#include <stdbool.h>
#include "socket_server.h"
#include "client_handler.h"

int main() {
    SOCKET server = create_server_socket(3000);
    SOCKET client_socket;
    int connections_remaining = MAX_CONNECTIONS;

    while(connections_remaining > 0) {
        client_socket = accept_client_socket(server);

        if(client_socket == INVALID_SOCKET) {
            printf("Failed to accept client. Retrying...\n");
            continue;
        }

        printf("\n\nHandling new client connection...");
        handle_connection(client_socket);
        closesocket(client_socket);

        connections_remaining--;
    }

    closesocket(server);
    WSACleanup();

    return 0;
}