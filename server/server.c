//
// Created by Victor on 26/10/2023.
//

#include <stdlib.h>
#include "net/config.h"
#include "server.h"


SOCKET clients[MAX_CLIENTS];

#include "net/handle_client.h"

int start_server() {

    for (int i = 0; i < MAX_CLIENTS; ++i) {
        clients[i] = INVALID_SOCKET;
    }

    SOCKET server_socket, new_socket;
    struct sockaddr_in address;
    int addr_len = sizeof(address);

#ifdef _WIN32
    WSADATA wsa_data;
    typedef int socklen_t;
    if (WSAStartup(MAKEWORD(2, 2), &wsa_data) != 0) {
        printf("Failed to initialize.\n");
        return 1;
    }
#endif

    if ((server_socket = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if (bind(server_socket, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    if (listen(server_socket, 3) < 0) {
        perror("listen failed");
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in sin;
    socklen_t len = sizeof(sin);
    if (getsockname(server_socket, (struct sockaddr *)&sin, &len) == -1) {
        perror("getsockname failed");
        exit(EXIT_FAILURE);
    } else {
        char *ip = inet_ntoa(sin.sin_addr);
        unsigned int port = ntohs(sin.sin_port);
        printf("Server listening on %s:%u\n", ip, port);
    }

    while (1) {
        if ((new_socket = accept(server_socket, (struct sockaddr *)&address, (socklen_t*)&addr_len)) < 0) {
            perror("accept failed");
            exit(EXIT_FAILURE);
        }

        printf("New connection: %s:%d\n", inet_ntoa(address.sin_addr), ntohs(address.sin_port));

        // Add the new client to the clients array and start a new thread for the client
        for (int i = 0; i < MAX_CLIENTS; i++) {
            if (clients[i] == INVALID_SOCKET) {
                clients[i] = new_socket;

#ifdef _WIN32 // Création du thread sur windows
                HANDLE threadHandle;
                threadHandle = CreateThread(NULL, 0, handle_client, &clients[i], 0, NULL);
                if (threadHandle != NULL) {
                    CloseHandle(threadHandle);
                }
#else // Création du thread sur unix
                pthread_t thread_id;
                pthread_create(&thread_id, NULL, handle_client, &clients[i]);
                pthread_detach(thread_id); // To automatically reclaim thread resources
#endif

                break;
            }
        }
    }

#ifdef _WIN32
    WSACleanup();
#endif

    return 0;
}