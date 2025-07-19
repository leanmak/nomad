#include <stdio.h>
#include "socket_server.h"
#include "client_handler.h"

int main() {
    SOCKET server = create_server_socket(3000);
    SOCKET client_socket;
    int result;

    while((client_socket = accept_client_socket(server)) != (long long unsigned int)-1) {
        printf("\n\nHandling new client connection...");

        result = handle_connection(client_socket);
        closesocket(client_socket);
    }

    closesocket(server);
    WSACleanup();

    return 0;
}