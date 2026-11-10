#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>

void *handle_recv_message(void *arg);

int main(){
    int client_socket = socket(AF_INET,
                               SOCK_STREAM, IPPROTO_TCP);
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(8080);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    connect(client_socket, (struct sockaddr *)&server_addr, sizeof(server_addr));
    
    pthread_t recv_thread;
    pthread_create(&recv_thread, NULL, handle_recv_message, &client_socket);

    while(1){
        char message[1024] = {0};
        fgets(message, sizeof(message), stdin);
        int bytes_sent = send(client_socket, message, strlen(message), 0);
        if (bytes_sent < 0) {
            perror("Error sending message");
            break;
        }
    }

    pthread_join(recv_thread, NULL);
    return 0;
}

void *handle_recv_message(void *arg){
    int client_socket = *(int *)arg;
    char buffer[1024];
    while(1){
        memset(buffer, 0, sizeof(buffer));
        int bytes_received = recv(client_socket, buffer, sizeof(buffer) - 1, 0);
        if (bytes_received <= 0) {
            printf("Server disconnected or error occurred.\n");
            break; 
        }
        printf("Received message: %s", buffer);
    }
    return NULL;
}