#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>

int main(){
    int listener = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(8080);
    server_addr.sin_addr.s_addr = INADDR_ANY;
    bind(listener, (struct sockaddr *)&server_addr, sizeof(server_addr));
    listen(listener, 5);

    fd_set read_fds;
    FD_SET(listener, &read_fds);
    int client_list[50] = {0};
    int number_of_clients = 0;
    int max_fd = listener;
    while(1){
        FD_ZERO(&read_fds);
        FD_SET(listener, &read_fds);
        for (int i = 0; i < number_of_clients; i++){
            FD_SET(client_list[i], &read_fds);
        }
        int activity = select(max_fd + 1, &read_fds, NULL, NULL, NULL);
        if (activity < 0){
            printf("Error in select()\n");
            break;
        } else {
            if (FD_ISSET(listener, &read_fds)){
                int client_socket = accept(listener, NULL, NULL);
                printf("New client connected: %d\n", client_socket);
                client_list[number_of_clients] = client_socket;
                number_of_clients++;
                FD_SET(client_socket, &read_fds);
                if (client_socket > max_fd){
                    max_fd = client_socket;
                }
            }
            for (int i = 0; i < number_of_clients; i++){
                int client_socket = client_list[i];
                if (FD_ISSET(client_socket, &read_fds)){
                    char buffer[1024];
                    int bytes_received = recv(client_socket, buffer, sizeof(buffer) - 1, 0);
                    if (bytes_received > 0){
                        buffer[bytes_received] = '\0';
                        printf("Received from client %d: %s", client_socket, buffer);
                        send(client_socket, buffer, bytes_received, 0);
                    } else if (bytes_received == 0){
                        printf("Client %d disconnected\n", client_socket);
                        close(client_socket);
                        FD_CLR(client_socket, &read_fds);
                    } else {
                        printf("Error in recv() from client %d\n", client_socket);
                        close(client_socket);
                        FD_CLR(client_socket, &read_fds);
                    }
                }
            }
        }
    }



    return 0;
}
