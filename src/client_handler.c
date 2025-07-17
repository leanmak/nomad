#include <stdio.h>
#include "client_handler.h"

int handle_connection(SOCKET client_socket) {
    // read connection content
    char buffer[1024];
    
    int bytes_recieved = recv(client_socket, buffer, sizeof(buffer), 0);
    if(bytes_recieved == SOCKET_ERROR) {
        int err = WSAGetLastError();
        printf("\nrecv() failed with an error code of: %d", err);

        closesocket(client_socket);
        WSACleanup();

        return -1;
    }

    // add null-terminator at end of buffer
    buffer[bytes_recieved] = '\0';
    
    return 0;
}