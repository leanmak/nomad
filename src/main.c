#include <stdio.h>
# include <winsock2.h>
#include <sys/types.h>

void main() {
    WSADATA wsaData;
    int iResult;

    iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
    if (iResult != 0) {
        printf("WSAStartup failed with error: %d\n", iResult);
        return;
    }

    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) { 
        printf("socket creation failed...\n"); 
        exit(0); 
    } 
    struct sockaddr_in server_address;

    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = htonl(INADDR_ANY);
    server_address.sin_port = htons(3149);

    int bind_res = bind(sockfd, (struct sockaddr*)&server_address, sizeof(server_address));
    
    if(bind_res != 0) {
        printf("\nSocket binding failed");
        return;
    }

    printf("\nMIGHT'VE binded socket to ADDRESS localhost:8080");

    int lisn_res = listen(sockfd, 1);

    if(lisn_res != 0) {
        printf("\nSocket listening failed");
        return;
    }

    printf("\nListening for client...");

    struct sockaddr client_addr;
    int len = sizeof(client_addr);

    SOCKET new_socket = accept(sockfd, &client_addr, &len);

    // HUH?
    if(new_socket < 0) {
        printf("\nfailed to accept socket");
        return;
    }

    printf("\nAccepted client...");
    closesocket(sockfd);
}