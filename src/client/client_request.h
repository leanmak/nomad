#pragma once

#ifndef CLIENT_REQUEST_H
#define CLIENT_REQUEST_H

#include "../utils.h"

typedef enum parsing_status_e {
    PARSING_STATUS_LINE,
    PARSING_HEADERS,
    PARSING_BODY
} ParsingStatus;

typedef struct http_status_s {
    char method[16];
    char route[256];
    char http_version[16];
} HTTPStatus;

typedef struct http_header_s {
    char name[256];
    char value[256];
} HTTPHeader;

typedef struct http_request_s {
    HTTPStatus *status;
    HTTPHeader *headers[MAX_HEADER_COUNT];
    int header_count;
    char *body;
} HTTPRequest;

/**
 * @brief Parses a request buffer into an `HTTPRequest` struct.
 * 
 * @return A pointer to an `HTTPRequest` struct if successful.
 * @return `NULL` if unsuccessful.
 */
HTTPRequest *ParseHTTPRequest(char *request_buffer);

#endif