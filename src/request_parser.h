#pragma once

#ifndef REQUEST_PARSER_H
#define REQUEST_PARSER_H

struct http_status {
    char method[16];
    char route[256];
    char http_version[16];
};

struct http_header {
    char *name;
    char *value;
};

struct http_request {
    struct http_status status;
    struct http_header headers[100];
    char *body;
};

struct http_header parse_header(char *header_line);
struct http_status parse_status(char *status_line);
struct http_request parse_request(char *request_buffer);

#endif