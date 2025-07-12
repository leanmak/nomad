#include <stdio.h>
# include <winsock2.h>
#include <sys/types.h>

void main() {
    // Start up Winsock API
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2,2), &wsaData) != 0) {
        printf("Failed to start up Winsock API.");
        return;
    }

    SOCKET server_socket;
    SOCKET client_socket;

    // Create socket
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket < 0) { 
        printf("socket creation failed...\n");
        WSACleanup();
        exit(0); 
    } 

    // Bind socket to IP address and port
    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = htonl(INADDR_LOOPBACK); // localhost (127.0.0.1)
    server_address.sin_port = htons(5000);

    if(bind(server_socket, (struct sockaddr*)&server_address, sizeof(server_address)) < 0) {
        printf("\nSocket binding failed");
        closesocket(server_socket);
        WSACleanup();
        return;
    }

    printf("Bound socket to: http://localhost:5000", server_address.sin_port);

    // Listen for incoming connections
    if(listen(server_socket, 5) < 0) {
        printf("\nsocket listening failed...");
        closesocket(server_socket);
        WSACleanup();
        return;
    }

    printf("\n\nListening for client...");

    // Accept client connection
    struct sockaddr client_addr;
    int len = sizeof(client_addr);

    client_socket = accept(server_socket, &client_addr, &len);
    if(client_socket < 0) {
        printf("\nfailed to accept socket");
        closesocket(server_socket);
        WSACleanup();
        return;
    }

    closesocket(server_socket);

    printf("\n\nAccepted client...");

    // Recieve and read bytes
    char buffer[1024];
    int res = recv(client_socket, buffer, 1024, 0);

    if(res > 0) {
        printf("\nRecieved bytes: %d", res);
        printf("\n%s\n", buffer);

        // send back 200 OK
        char *response = "HTTP/1.1 200 OK\r\n\r\n";
        int bytes_sent = send(client_socket, response, res, 0);
        if(bytes_sent < 0) {
            printf("\nfailed to send response...");
        } else {
            printf("\n\nSent %d bytes\n\n", bytes_sent);
        }
    } else {
        printf("\nfailed to recieve bytes...");
    }

    printf("\nending execution...\n");
}