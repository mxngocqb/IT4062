#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/ioctl.h>

#define MAX_CLIENTS 10
int io_mode = 1;

int main(){
    int listener = socket(
        AF_INET, 
        SOCK_STREAM,
        IPPROTO_TCP
    );

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(8080);
    addr.sin_addr.s_addr = INADDR_ANY;

    int opt = 1;
    setsockopt(listener, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    if (bind(
            listener, 
            (struct sockaddr*)&addr, 
            sizeof(addr)
        ) == -1
    ){
        perror("bind() failed");
        return 1;
    }

    if (listen(listener, 5) == -1){
        perror("listen() failed");
        return 1;
    }

    char buffer[1024] = {0};
    int current_client = 0;
    int client_sockets[MAX_CLIENTS] = {0};
    int num_clients = 0;

    ioctl(listener, FIONBIO, &io_mode);

    while (0 == 0){
        int client = accept(
            listener, 
            NULL, 
            NULL
        );

        if (client == -1){
            // No new client, continue to check existing clients
        } else {
            if (num_clients < MAX_CLIENTS) {
                client_sockets[num_clients++] = client;
                ioctl(client, FIONBIO, &io_mode);
                printf("New client connected: %d\n", client);
            } else {
                printf("Max clients reached. Rejecting new connection.\n");
                close(client);
            }
        }

        for (int i = 0; i < num_clients; i++){
            int byte_received = recv(
                client_sockets[i], 
                buffer, 
                sizeof(buffer), 
                0
            );
            if (byte_received > 0){
                printf("Received %d bytes from client %d: %s\n", byte_received, client_sockets[i], buffer);
                char message[] = "Hello from server!\r\n";
                send(client_sockets[i], message, strlen(message), 0);
            } else if (byte_received == 0) {
                printf("Client %d disconnected.\n", client_sockets[i]);
                close(client_sockets[i]);
            } else if (byte_received == -1) {
                continue; // No data received, continue to next client
            }
        }
    }

}