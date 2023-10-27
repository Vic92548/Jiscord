//
// Created by Victor on 26/10/2023.
//

#ifndef SERVER_CONFIG_H
#define SERVER_CONFIG_H

#include <stdio.h>

#ifdef _WIN32
#include <winsock2.h>
#include <windows.h>
#pragma comment(lib, "ws2_32.lib")
#else
#define SOCKET int
    #define INVALID_SOCKET (-1)
    #define SOCKET_ERROR (-1)
    #define closesocket close
    #include <sys/socket.h>
    #include <netinet/in.h>
    #include <arpa/inet.h>
    #include <unistd.h>
    #include <pthread.h>
#endif

#define PORT 8080
#define MAX_CLIENTS 5
#define BUFFER_SIZE 1024

extern SOCKET clients[MAX_CLIENTS];

#endif //SERVER_CONFIG_H
