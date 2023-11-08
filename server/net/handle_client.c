//
// Created by Victor on 26/10/2023.
//

#include <string.h>
#include "handle_client.h"

void sendMessage(char buffer[]){
    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (clients[i] != INVALID_SOCKET) {
            send(clients[i], buffer, strlen(buffer), 0);
        }
    }
}

// COMMANDS
void test(char message[]){
    printf("Nouveau message : %s\n", message);

    char result[100];
    snprintf(result, sizeof(result), "Result of test: %s", message);
    sendMessage(result);
}


#ifdef _WIN32
DWORD WINAPI handle_client(LPVOID client_socket) {
#else
    void* handle_client(void* client_socket) {
#endif
    SOCKET socket = *(SOCKET*)client_socket;
    char buffer[BUFFER_SIZE];

    printf("New thread ready!\n");

    while (1) {
        int bytes_received = recv(socket, buffer, BUFFER_SIZE, 0);
        if (bytes_received <= 0) {
            break;
        }

        buffer[bytes_received] = '\0';
        printf("Received message: %s\n", buffer);

        // Analyze the message

        if (buffer[0] == '/') {
            char command_name[50] = {};
            int last_command_name_index = 0;
            char arg_1[50] = {};
            int last_arg_1_index = 0;
            int command_found = 0;

            for (int i = 1; i < bytes_received; ++i) {
                if (buffer[i] == ' '){
                    if(command_found == 1){
                        arg_1[last_arg_1_index] = buffer[i];
                        last_arg_1_index++;
                    }
                    command_found = 1;
                }else if(command_found == 1){
                    arg_1[last_arg_1_index] = buffer[i];
                    last_arg_1_index++;
                }else{
                    command_name[last_command_name_index] = buffer[i];
                    last_command_name_index++;
                }
            }

            printf("Command received: '%s'", command_name);
            printf(" with argument : '%s'\n", arg_1);

            if(strcmp(command_name, "test") == 0){
                test(arg_1);
            }
        }else{
            sendMessage(buffer);
        }


// Send the message to all connected clients.

    }

    closesocket(socket);
    return 0;
}


