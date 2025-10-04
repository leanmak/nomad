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

char *GetFileContent(char *file_path) {
    FILE *file = fopen(file_path, "rb");
    if(!file) {
        printf("Failed to get the file content at %s\n", file_path);
        return NULL;
    }

    // Get file size/length by moving the pointer to the end of the file.
    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    rewind(file);

    char *content = malloc(size+1);
    if(!content) {
        printf("Failed to allocate memory for the file content.\n");
        return NULL;
    }

    fread(content, 1, size, file);
    content[size] = '\0';

    fclose(file);

    return content;
}