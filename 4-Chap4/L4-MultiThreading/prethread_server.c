#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>

#define MAX_CLIENTS 10
void *handle_client(void *arg);

int main(){
    int listener = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(8080);
    server_addr.sin_addr.s_addr = INADDR_ANY;
    bind(listener, (struct sockaddr *)&server_addr, sizeof(server_addr));
    listen(listener, MAX_CLIENTS);
    printf("Server is listening on port 8080...\n");
    for (int i = 0; i < MAX_CLIENTS; i++){
        pthread_t thread_id;
        pthread_create(&thread_id, NULL, handle_client, (void *)(intptr_t)listener);
        pthread_detach(thread_id);
        printf("Thread %d created to handle client connections.\n", (int)thread_id);
    }

    while (1){
        sleep(1);
    }

    return 0;
}

void *handle_client(void *arg){
    int listenner = (intptr_t)arg;
    char buffer[1024] = {0};
    while (1){
        int client_socket = accept(listenner, NULL, NULL);
        memset(buffer, 0, sizeof(buffer));
        int bytes_received = recv(client_socket, buffer, sizeof(buffer), 0);
        if (bytes_received <= 0){
            printf("Client disconnected or error occurred.\n");
            close(client_socket);
            continue;
        }
        buffer[bytes_received] = '\0';
        printf("Received from client: %s", buffer);
    }
}

