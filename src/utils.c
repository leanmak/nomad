#include <stdio.h>
#include <winsock2.h>

#include "utils.h"

void InitWSA() {
    WSADATA wsa_data;

    if(WSAStartup(MAKEWORD(2, 2), &wsa_data) != NO_ERROR) {
        exit(EXIT_FAILURE);
    }
}

void *xmalloc(size_t bytes) {
    void *new_memory = malloc(bytes);
    if(!new_memory) {
        exit(EXIT_FAILURE);
    }

    return new_memory;
}