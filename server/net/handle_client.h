//
// Created by Victor on 26/10/2023.
//

#include "config.h"

#ifndef SERVER_HEANDLE_CLIENT_H
#define SERVER_HEANDLE_CLIENT_H

#ifdef _WIN32
DWORD WINAPI handle_client(LPVOID client_socket);
#else
void* handle_client(void* client_socket);
#endif

#endif //SERVER_HEANDLE_CLIENT_H
