#include <stdio.h>
#include "request_parser.h"
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>

struct http_header parse_header(char *header_line) {
    struct http_header header;

    // find first colon and set it as the end of the string (splits it)
    char *colon = strchr(header_line, ':');
    *colon = '\0'; 

    header.name = header_line;

    // remove whitespace
    colon++;
    while(colon == (char*)' ') colon++;
    header.value = colon+1;

    return header;
}

struct http_status parse_status(char *status_line) {
    struct http_status status;

    char *status_i = strtok(status_line, " ");
    int i = 0;
    while(status_i != NULL) {
        if (i == 0) {
            strncpy(status.method, status_i, sizeof(status.method) - 1);
        } else if (i == 1) {
            strncpy(status.route, status_i, sizeof(status.route) - 1);
        } else {
            strncpy(status.http_version, status_i, sizeof(status.http_version) - 1);
        }

        status_i = strtok(NULL, " ");
        i++;
    }

    return status;
}

struct http_request parse_request(char *request_buffer) {
    struct http_request request;

    //printf("\n%s", request_buffer);

    /*
        0 = reading status
        1 = reading headers
    */
    int reading = 0;
    int next_header_index = 0;
    int body_length;

    // have to use strtok_r to manage the tracking myself instead of passing it to internal static state :(
    char *save_ptr;
    char *request_i = strtok_r(request_buffer, "\n", &save_ptr);

    while(request_i != NULL) {
        // create copy temp string
        int length = strlen(request_i);
        char *tmp = (char*)malloc((length+1) * sizeof(char));
        strncpy(tmp, request_i, length);
        tmp[length] = '\0';

        // Status Line
        if(reading == 0) {
            request.status = parse_status(tmp);
            reading++;
        } else { // Headers
            // skip to body
            if(length == 1) {
                free(tmp);
                request_i = strtok_r(NULL, "\n", &save_ptr);
                break;
            }
            struct http_header h = parse_header(tmp);

            if(_stricmp(h.name, "Content-Length") == 0) {
                body_length = atoi(h.value);
            }

            request.headers[next_header_index++] = h;
        }

        free(tmp);
        request_i = strtok_r(NULL, "\n", &save_ptr);
    }

    // TODO: remove spaces
    // char *body = (char*)malloc((body_length+1)*sizeof(char));
    // body[0] = '\0';
    // while(request_i != NULL) {
    //     int length = strlen(request_i);
    //     strncat(body, request_i, length);
    //     request_i = strtok_r(NULL, "\n", &save_ptr);
    // }
    // request.body = body;

    return request;
}