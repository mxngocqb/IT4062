#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>

int main(){
    int client_socket = socket(AF_INET,
                               SOCK_STREAM, IPPROTO_TCP);
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(8080);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    connect(client_socket, (struct sockaddr *)&server_addr, sizeof(server_addr));
    
    fd_set read_fds;
    struct timeval timeout;
    timeout.tv_sec = 5; // Set timeout to 5 seconds
    timeout.tv_usec = 0;
    while(1){
        FD_ZERO(&read_fds);
        FD_SET(client_socket, &read_fds);
        FD_SET(STDIN_FILENO, &read_fds);
        int max_fd = (client_socket > STDIN_FILENO) ? client_socket : STDIN_FILENO;
        int activity = select(
            max_fd + 1, 
            &read_fds, 
            NULL, 
            NULL, 
            &timeout
        );
        
        if (activity < 0){
            printf("Error in select()\n");
            break;
        } else {
            if (FD_ISSET(STDIN_FILENO, &read_fds)){
                char buffer[1024];
                fgets(buffer, sizeof(buffer), stdin);
                send(client_socket, buffer, strlen(buffer), 0);
            }

            if (FD_ISSET(client_socket, &read_fds)){
                char buffer[1024];
                int bytes_received = recv(client_socket, buffer, sizeof(buffer) - 1, 0);
                if (bytes_received > 0){
                    buffer[bytes_received] = '\0';
                    printf("Received from server: %s", buffer);
                } else if (bytes_received == 0){
                    printf("Server closed the connection\n");
                    break;
                } else {
                    printf("Error in recv()\n");
                    break;
                }
            }
        }
        
    }

    return 0;
}
