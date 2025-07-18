#include <stdio.h>
#include "client_handler.h"
#include "request_parser.h"
#include <string.h>
#include <math.h>

// Reads the specified file's content into a string
char* read_file_to_string(char *file_name) {
    FILE *file = fopen(file_name, "rb"); // "rb" for byte accuracy
    if(file == NULL) {
        return NULL;
    }

    // Get file size by moving pointer to end of the file
    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    rewind(file);

    // Read file content into string
    char *buffer = (char*)malloc(size + 1);
    fread(buffer, 1, size, file);
    buffer[size] = '\0';

    fclose(file);

    return buffer;
}

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
    if(strcmp(request.status.route, "/") == 0) {
        char *file_content = read_file_to_string("index.html");
        if (file_content == NULL) {
            printf("\nFailed to fetch file content.");

            closesocket(client_socket);
            WSACleanup();

            return -1;
        }

        int content_length = strlen(file_content);
        char response_header[256];

        snprintf(response_header, sizeof(response_header), "HTTP/1.1 200 OK\r\nContent-Length: %d\r\nContent-Type: text/html\r\n\r\n", content_length); 

        send(client_socket, response_header, strlen(response_header), 0);
        send(client_socket, file_content, content_length, 0);

        free(file_content);
    } else {
        printf("\nNOT A ROUTE\n%s",request.status.route);
    }

    return 0;
}
