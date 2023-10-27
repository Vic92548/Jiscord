//
// Created by Victor on 26/10/2023.
//

#include "handle_client.h"

#ifdef _WIN32
DWORD WINAPI handle_client(LPVOID client_socket) {
#else
    void* handle_client(void* client_socket) {
#endif
    SOCKET socket = *(SOCKET*)client_socket;
    char buffer[BUFFER_SIZE];

    while (1) {
        int bytes_received = recv(socket, buffer, BUFFER_SIZE, 0);
        if (bytes_received <= 0) {
            break;
        }

        buffer[bytes_received] = '\0';
        printf("Received message: %s\n", buffer);

// Send the message to all connected clients.
        for (int i = 0; i < MAX_CLIENTS; i++) {
            if (clients[i] != INVALID_SOCKET) {
                send(clients[i], buffer, bytes_received, 0);
            }
        }
    }

    closesocket(socket);
    return 0;
}


