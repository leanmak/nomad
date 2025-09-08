#include <stdio.h>
#include <winsock2.h>
#include <string.h>

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

char *DuplicateCRLFString(const char *s) {
    char *duplicate = strdup(s);
    if(!duplicate) {
        printf("Failed to duplicate CRLF string.\n");
        return NULL;
    }

    // remove trailing '\r' character
    size_t len = strlen(s);
    if(len > 0 && duplicate[len-1] == '\r') duplicate[len-1] = '\0';

    return duplicate;
}

void FreeClientContext(ClientContext *ctx) {
    if(ctx->buffer) {
        free(ctx->buffer);
    }
    
    free(ctx);
}

void FreeHTTPRequest(HTTPRequest *req) {
    if(req->status) {
        free(req->status);
    }

    for(int i = 0; i < req->header_count; i++) {
        free(req->headers[i]);
    }

    if(req->body)  {
        free(req->body);
    }

    free(req);
}