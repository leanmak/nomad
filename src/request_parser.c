#include <stdio.h>
#include "request_parser.h"
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>

// Parses the status line of an http request (e.g. GET /about HTTP/1.1)
struct http_status parse_status(char *status_line) {
    struct http_status status;
    char *status_i = strtok(status_line, " ");

    int i = 0;
    while(status_i != NULL) {
        int length = strlen(status_i);

        if(i == 0) {
            strncpy(status.method, status_i, length);
            status.method[length] = '\0';
        } else if(i == 1) {
            strncpy(status.route, status_i, length);
            status.route[length] = '\0';
        } else {
            strncpy(status.http_version, status_i, length);
            status.http_version[length] = '\0';
        }

        i++;
        status_i = strtok(NULL, " ");
    }

    return status;
}

// Parses the header line of an http request (e.g. HeaderName: HeaderValue)
struct http_header parse_header(char *header_line) {
    struct http_header header;

    char *colon = strchr(header_line, ':');
    *colon = '\0';
    
    int header_name_length = strlen(header_line);
    strncpy(header.name, header_line, header_name_length);
    header.name[header_name_length] = '\0';

    // everything after colon is the header value (but there are some trailing whitespaces)
    colon++;
    while(*colon == ' ') colon++;
    int header_value_length = strlen(colon);
    strncpy(header.value, colon, header_value_length);
    header.value[header_value_length] = '\0';

    return header;
}

// Parses the entire http request (Status Line + Headers + Body)
struct http_request parse_request(char *request_buffer) {
    struct http_request request;
    char *save_ptr;
    char *request_i = strtok_r(request_buffer, "\n", &save_ptr);

    int reading = 0;
    int next_header_idx = 0;
    int content_length = 0;
    while(request_i != NULL) {
        char *tmp = strdup(request_i);

        if(reading == 0) {
            request.status = parse_status(tmp);
            reading++;
        } else if(reading == 1) {
            // pass to body
            if(strlen(request_i) == 1) {
                request_i = strtok_r(NULL, "\n", &save_ptr);
                if(content_length > 0) reading++;
                request.header_count = next_header_idx;
                continue;
            }

            struct http_header h = parse_header(tmp);
            if(_stricmp(h.name, "content-length") == 0) content_length = atoi(h.value);
            request.headers[next_header_idx++] = h;
        } else if(reading == 2) {
            int length = strlen(tmp);

            char *buffer = (char*)malloc(length+1);
            sprintf(buffer, "%s\n", tmp);

            strcat(request.body, buffer);
        }

        request_i = strtok_r(NULL, "\n", &save_ptr);
    }

    request.body[content_length] = '\0';

    return request;
}
