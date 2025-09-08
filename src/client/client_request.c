#include <stdio.h>
#include <winsock2.h>
#include <stdbool.h>

#include "client_request.h"

/**
 * @brief Parses a status line into an `HTTPStatus` struct.
 * 
 * @return A pointer to an `HTTPStatus` struct if successful.
 * @return `NULL` if unsuccessful.
 */
static HTTPStatus *ParseHTTPStatus(char *status_line);

/**
 * @brief Parses a header line into a `HTTPHeader` struct.
 * 
 * @return A pointer to an `HTTPHeader` struct if successful.
 * @return `NULL` if unsuccessful.
 */
static HTTPHeader *ParseHTTPHeader(char *header_line);

HTTPRequest *ParseHTTPRequest(char *request_buffer) {
    HTTPRequest *request = malloc(sizeof(HTTPRequest));
    if(!request) {
        printf("Failed to allocate memory for an HTTPRequest struct.\n");
        return NULL;
    }

    ZeroMemory(request, sizeof(HTTPRequest));

    char *tmp_body = strdup(request_buffer);
    if(!tmp_body) {
        printf("Failed to duplicate request buffer.\n");
        FreeHTTPRequest(request);
        return NULL;
    }

    ParsingStatus status = PARSING_STATUS_LINE;
    int content_length = 0;

    char *save_ptr;
    char *line = strtok_r(request_buffer, "\n", &save_ptr);

    // parsing status line
    while(status == PARSING_STATUS_LINE) {
        char *tmp = DuplicateCRLFString(line);
        if(!tmp) {
            free(tmp_body);
            FreeHTTPRequest(request);
            return NULL;
        }

        request->status = ParseHTTPStatus(tmp);
        if(!request->status) {
            free(tmp_body);
            free(tmp);
            FreeHTTPRequest(request);
            return NULL;
        }

        status = PARSING_HEADERS;

        free(tmp);
        line = strtok_r(NULL, "\n", &save_ptr);
    }

    // printf("\n=== STATUS ===\nRoute: %s\nMETHOD: %s\nVERSION: %s\n", request->status->route, request->status->method, request->status->http_version);

    // parsing headers
    while(status == PARSING_HEADERS && line != NULL) {
        char *tmp = DuplicateCRLFString(line);
        if(!tmp) {
            free(tmp_body);
            FreeHTTPRequest(request);
            return NULL;
        }

        // Empty Line == Start of Body
        if(strlen(tmp) == 0) {
            free(tmp);

            line = strtok_r(NULL, "\n", &save_ptr);
            status = PARSING_BODY;

            break;
        }

        if(request->header_count < MAX_HEADER_COUNT) {
            HTTPHeader *header = ParseHTTPHeader(tmp);
            if(!header) {
                free(tmp_body);
                FreeHTTPRequest(request);
                free(tmp);
                return NULL;
            }

            if(_stricmp(header->name, "content-length") == 0) content_length = atoi(header->value);

            request->headers[request->header_count++] = header;
        }
        
        free(tmp);
        line = strtok_r(NULL, "\n", &save_ptr);
    }

    // printf("\n=== HEADERS (Count: %d) ===", request->header_count);
    // for(int i = 0; i < request->header_count; i++) {
    //     HTTPHeader *header = request->headers[i];
    //     printf("\n%s: %s", header->name, header->value);
    // }

    // Read Body (only works for simple text payloads)
    char *body_start = strstr(tmp_body, "\r\n\r\n");
    if (body_start) {
        body_start += 4; // skip past the "\r\n\r\n"
    }

    request->body = malloc(content_length + 1);
    if (!request->body) {
        free(tmp_body);
        FreeHTTPRequest(request);
        printf("Failed to allocate memory for request body.\n");
        return NULL;
    }

    memcpy(request->body, body_start, content_length);
    request->body[content_length] = '\0';

    // printf("\n=== BODY ===\n%s", request->body);

    free(tmp_body);
    return request;
}

static HTTPStatus *ParseHTTPStatus(char *status_line) {
    HTTPStatus *status = malloc(sizeof(HTTPStatus));
    if(!status) {
        printf("Failed to allocate memory for an HTTPStatus struct.");
        return NULL;
    }

    char *status_i = strtok(status_line, " ");

    int i = 0;
    while(status_i != NULL) {
        if(i == 0) {
            // it's better to copy size-1 of the field to avoid overflow.
            strncpy(status->method, status_i, sizeof(status->method) - 1);
            status->method[sizeof(status->method) - 1] = '\0';
        } else if(i == 1) {
            strncpy(status->route, status_i, sizeof(status->route) - 1);
            status->route[sizeof(status->route) - 1] = '\0';
        } else {
            strncpy(status->http_version, status_i, sizeof(status->http_version) - 1);
            status->http_version[sizeof(status->http_version) - 1] = '\0';
        }

        i++;
        status_i = strtok(NULL, " ");
    }

    return status;
}

static HTTPHeader *ParseHTTPHeader(char *header_line) {
    HTTPHeader *header = malloc(sizeof(HTTPHeader));
    if(!header) {
        printf("Failed to allocate memory for an HTTPHeader struct.");
        return NULL;
    }

    char *colon = strchr(header_line, ':');
    if (!colon) {
        free(header);
        return NULL;
    }
    *colon = '\0';

    // copy header name
    strncpy(header->name, header_line, sizeof(header->name) - 1);
    header->name[sizeof(header->name) - 1] = '\0';

    // move past colon and skip spaces
    colon++;
    while (*colon == ' ') colon++;

    // copy header value
    strncpy(header->value, colon, sizeof(header->value) - 1);
    header->value[sizeof(header->value) - 1] = '\0';

    return header;
}