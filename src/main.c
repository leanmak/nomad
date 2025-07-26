#include <stdio.h>
#include <stdbool.h>
#include <pthread.h>
#include "main.h"
#include "socket_server.h"
#include "client_handler.h"

int main() {
    SOCKET server = create_server_socket(3000);
    SOCKET client_socket;
    int count = 1;

    while(true) {
        client_socket = accept_client_socket(server);

        if(client_socket == INVALID_SOCKET) {
            printf("Failed to accept client. Retrying...\n");
            continue;
        }

        // Allocate space for a new socket
        SOCKET* client_socket_ptr = malloc(sizeof(SOCKET));
        *client_socket_ptr = client_socket;

        // Create detached thread per connection
        pthread_t thread;
        pthread_attr_t attr;
        pthread_attr_init(&attr);
        pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

        struct handle_connection_args* args = malloc(sizeof(struct handle_connection_args));
        args->id = count++;
        args->client_socket_ptr = client_socket_ptr;

        if(pthread_create(&thread, &attr, handle_connection, (void*)args) != 0) {
            printf("Failed to create thread for client. Retrying...\n");
            free(client_socket_ptr);
            free(args);
            continue;
        }

        // cleanup
        pthread_attr_destroy(&attr);
    }

    return 0;
}