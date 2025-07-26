#include <stdio.h>
#include <stdbool.h>
#include <pthread.h>
#include "socket_server.h"
#include "client_handler.h"

int main() {
    SOCKET server = create_server_socket(3000);
    SOCKET client_socket;

    while(true) {
        client_socket = accept_client_socket(server);

        if(client_socket == INVALID_SOCKET) {
            printf("Failed to accept client. Retrying...\n");
            continue;
        }

        // Create detached thread per connection
        pthread_t thread;
        pthread_attr_t attr;

        pthread_attr_init(&attr);
        pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

        if(pthread_create(&thread, &attr, handle_connection_test, (void*)&client_socket) != 0) {
            printf("Failed to create thread for client. Retrying...\n");
            continue;
        }

        // cleanup
        pthread_attr_destroy(&attr);
    }

    return 0;
}