#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netdb.h>
#include <string.h>
#include <errno.h>

int main(){
    // Tạo Socket
    int listener = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (listener == -1) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(8080);
    addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(listener, (struct sockaddr*)&addr, sizeof(addr)) == -1) {
        perror("bind");
        close(listener);
        exit(EXIT_FAILURE);
    }

    if (listen(listener, 5) == -1) {
        perror("listen");
        close(listener);
        exit(EXIT_FAILURE);
    }

    printf("Server is listening on port 8080...\n");

    int client_socket = accept(listener, NULL, NULL);
    if (client_socket == -1) {
        perror("accept");
        close(listener);
        exit(EXIT_FAILURE); 
    }

    printf("Client connected!\n: %d", client_socket);

    char buffer[1024];
    ssize_t bytes_received = recv(client_socket, buffer, sizeof(buffer) - 1, 0);
    if (bytes_received == -1) {
        perror("recv");
        close(client_socket);
        close(listener);
        exit(EXIT_FAILURE); 
    }

    buffer[bytes_received] = '\0'; // Null-terminate the received data
    printf("Received: %s\n", buffer);

    send(client_socket, buffer, bytes_received, 0);

    close(client_socket);
    close(listener);
    return 0;
}