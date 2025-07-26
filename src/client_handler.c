#include <stdio.h>
#include "client_handler.h"
#include "request_parser.h"
#include "main.h"
#include <string.h>
#include <Windows.h>

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
    if(!buffer) {
        printf("\nFailed to allocate memory for file content buffer.");
        return NULL;
    }

    fread(buffer, 1, size, file);
    buffer[size] = '\0';
    
    fclose(file);
    
    return buffer;
}

int serve_page(char *status_line, char *content_type, char *file_name, SOCKET *client_socket) {
    char *file_content = read_file_to_string(file_name);
    if (file_content == NULL) {
        printf("\nFailed to fetch '%s' content.", file_name);
        return -1;
    }
    
    int content_length = strlen(file_content);
    char response_header[256];
    
    snprintf(response_header, sizeof(response_header), "%s\r\nContent-Length: %d\r\nContent-Type: %s\r\n\r\n", status_line, content_length, content_type); 
    
    int result;
    int error;
    result = send(*client_socket, response_header, strlen(response_header), 0);
    if(result == SOCKET_ERROR) {
        error = WSAGetLastError();
        printf("\nsend() failed to send response header with an error code of: %d", error);
        
        return -1;
    }

    result = send(*client_socket, file_content, content_length, 0);
    if(result == SOCKET_ERROR) {
        error = WSAGetLastError();
        printf("\nsend() failed to send response body with an error code of: %d", error);
        
        return -1;
    }

    free(file_content);

    // practical convention + flushes send() data that might be in a buffer
    shutdown(*client_socket, SD_SEND);

    return 0;
}

void* handle_connection(void* args) {
    struct handle_connection_args* sargs = (struct handle_connection_args*)args;

    int id = sargs->id;
    printf("\n\nHandling new client connection (ID: %d)...", id);

    int error;
    SOCKET* client_socket_ptr = sargs->client_socket_ptr;
    SOCKET client_socket = *client_socket_ptr;

    // read connection content
    char buffer[1024];
    int bytes_received = recv(client_socket, buffer, 1024, 0);
    if(bytes_received == SOCKET_ERROR) {
        error = WSAGetLastError();
        printf("\nrecv() failed with an error code of: %d", error);

        return NULL;
    }

    buffer[bytes_received] = '\0';

    struct http_request request = parse_request(buffer);

    // For now, serve either home or error page.
    if(strcmp(request.status.route, "/") == 0) {
        error = serve_page("HTTP/1.1 200 OK", "text/html", "index.html", client_socket_ptr);
    } else {
        error = serve_page("HTTP/1.1 404 Not Found", "text/html", "404.html", client_socket_ptr);
    }

    if(error == -1) {
        printf("\nFailed to serve page (ID: %d)", id);
    }

    printf("\n\nClosing client connection (ID: %d)...", id);

    closesocket(client_socket);
    free(client_socket_ptr);
    free(args);

    return NULL;
}
