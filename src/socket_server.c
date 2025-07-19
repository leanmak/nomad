#include <stdio.h>
#include "socket_server.h"
#include "client_handler.h"

bool init_wsa = false;
const int MAX_CONNECTIONS = 5192;

/*
Starts up the Winsock API which is needed to use any socket functions on Windows,
because Windows wants to be different and not automatically startup their socket system >:(
*/
int startup_winsock_api() {
    WSADATA wsa_data;
    if (WSAStartup(MAKEWORD(2,2), &wsa_data) != 0) {
        printf("\nFailed to start up Winsock API.");
        return -1;
    }

    init_wsa = true;
    return 0;
}

// Creates and binds a server socket to the loopback IP address and an arbitrary port.
SOCKET create_server_socket(int port) {
    if(!init_wsa && startup_winsock_api() < 0) {
        return -1;
    }

    // AF_INET to allow different hosts to connect by IPv4
    // SOCK_STREAM to use TCP as a transportation protocol
    SOCKET server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if(server_socket == INVALID_SOCKET) {
        int err = WSAGetLastError();
        printf("\nsocket() failed with an error code of: %d", err);

        return -1;
    }

    int result;

    // Setup socket address server to bind to address and port (sockaddr_in is IPv4-specific, so it's needed here).
    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET; // IPv4
    server_address.sin_addr.s_addr = htonl(INADDR_LOOPBACK); // localhost (127.0.0.1)
    server_address.sin_port = htons(port);
    
    result = bind(server_socket, (struct sockaddr*)&server_address, sizeof(server_address));
    if(result == SOCKET_ERROR) {
        int err = WSAGetLastError();
        printf("\nbind() failed with an error code of: %d", err);

        closesocket(server_socket);
        WSACleanup();

        return -1;
    }

    printf("\nBound server socket to: http://localhost:%d", port);

    // Listen for a specific amount of connections.
    result = listen(server_socket, MAX_CONNECTIONS);
    if(result == SOCKET_ERROR) {
        int err = WSAGetLastError();
        printf("\nlisten() failed with an error code of: %d", err);

        closesocket(server_socket);
        WSACleanup();

        return -1;
    }

    return server_socket;
}

// Accepts and returns a client socket.
int accept_client_socket(SOCKET server_socket) {
    SOCKET client_socket;

    struct sockaddr client_socket_addr;
    int len = sizeof(client_socket_addr);

    client_socket = accept(server_socket, &client_socket_addr, &len);
    if(client_socket == INVALID_SOCKET) {
        int err = WSAGetLastError();
        printf("\naccept() failed with an error code of: %d", err);

        closesocket(server_socket);
        WSACleanup();

        return -1;
    }

    return client_socket;
}