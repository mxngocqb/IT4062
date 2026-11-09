#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>

void *handle_keyboard_input(void *arg) {
    int client_socket = *(int *)arg;
    char buffer[1024];
    while (1) {
        fgets(buffer, sizeof(buffer), stdin);
        int bytes_sent = send(client_socket, buffer, strlen(buffer), 0);
        if (strncmp(buffer, "exit", 4) == 0) {
            printf("Exiting client.\n");
            break;
        }
        if (bytes_sent < 0) {
            printf("Error sending message.\n");
            break;
        }
    }
    return NULL;
}

int main() {
    // Create a socket
    int client_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(8080);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    connect(client_socket, (struct sockaddr *)&server_addr, sizeof(server_addr));
    char buffer[1024];
    
    pthread_t input_thread;
    pthread_create(&input_thread, NULL, handle_keyboard_input, &client_socket);

    while (1){
        int bytes_received = recv(client_socket, buffer, sizeof(buffer), 0);
        if(bytes_received <= 0){
            printf("Server disconnected or error occurred.\n");
            break;
        }
        buffer[bytes_received] = '\0';
        printf("Received from server: %s", buffer);
    }

    pthread_join(input_thread, NULL);
    return 0;
}