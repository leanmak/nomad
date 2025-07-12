#include <stdio.h>
# include <winsock2.h>
#include <sys/types.h>
#include <string.h>

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
    if(listen(server_socket, 1) < 0) {
        printf("\nsocket listening failed...");
        closesocket(server_socket);
        WSACleanup();
        return;
    }

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

    // Recieve and read bytes
    char buffer[1024];
    int res = recv(client_socket, buffer, 1024, 0);

    if(res > 0) {
        char tmp_buffer[1024];
        strncpy(tmp_buffer, buffer, 1024);
        char *sep = strtok(tmp_buffer, "\r\n"); // gives us the request line (first line)

        char *url_path = strtok(sep, " ");
        int i = 0;
        while(url_path != NULL && i < 1) {
            i++;
            url_path = strtok(NULL, " ");
        }

        url_path = strtok(url_path, "/");

        // send back 200 OK if home page, otherwise send not found
        char *response = (char*)malloc(1024 * sizeof(char));
        if(url_path == NULL) {
            response = "HTTP/1.1 200 OK\r\n\r\n";
        } else if(strcmp(url_path, "echo") == 0) { // /echo/{str}
            url_path = strtok(NULL, "/"); // get {str}
            if(url_path == NULL) {
                url_path = "";
            }

            sprintf(response, "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\nContent-Length: %d\r\n\r\n%s", strlen(url_path), url_path);
        } else if(strcmp(url_path, "user-agent") == 0) { // /user-agent
            tmp_buffer[0] = '\0';
            strncpy(tmp_buffer, buffer, 1024);

            char *headers = strtok(tmp_buffer, "\r\n");
            while(headers != NULL) {
                if(_strnicmp(headers, "user-agent", 10) == 0) {
                    break;
                }
                headers = strtok(NULL, "\r\n");
            }

            // skip "User-Agent: "
            char *start = headers + 12;

            // reach the first character of CRLF
            char *end = start;
            while(*end != '\n' && *end != '\r' && *end != '\0') {
                end++;
            }

            size_t user_agent_size = end - start;
            char *user_agent = (char*)malloc((len+1) * sizeof(char)); // +1 for the null terminator
            strncpy(user_agent, start, user_agent_size);
            user_agent[user_agent_size] = '\0';

            sprintf(response, "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\nContent-Length: %d\r\n\r\n%s", strlen(user_agent), user_agent);
        } else {
            response = "HTTP/1.1 404 Not Found\r\n\r\n";
        }

        send(client_socket, response, strlen(response), 0);
    } else {
        printf("\nfailed to recieve bytes...");
    }
}