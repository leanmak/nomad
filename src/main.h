#pragma once

#ifndef MAIN_H
#define MAIN_H

#include <winsock2.h>

struct handle_connection_args {
    int id;
    SOCKET* client_socket_ptr;
};

#endif