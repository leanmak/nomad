#include <stdio.h>
#include "client_handler.h"
#include "request_parser.h"
#include <string.h>

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

    struct http_request request = parse_request(buffer);
    
    // For now, serve either home or error page.
    FILE *file;

    if(strcmp(request.status.route, "/") == 0) {
        file = fopen("index.html", "r");
        if(file == NULL) {
            printf("\nFile doesn't exist.");
            return -1;
        }

        // read entire content into buffer
        fseek(file, 0, SEEK_END);
        long size = ftell(file);
        rewind(file);

        char *buffer = malloc(size + 1);
        fread(buffer, 1, size, file);
        buffer[size] = '\0';

        fclose(file);

        int result = send(client_socket, buffer, size, 0);
        if(result == SOCKET_ERROR) {
            printf("\nFailed to send page.");
            return -1;
        }
    } else {
        printf("\nNOT A ROUTE\n%s",request.status.route);
    }

    return 0;
}