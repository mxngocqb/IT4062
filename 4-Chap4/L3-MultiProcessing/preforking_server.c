#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/signal.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <errno.h>

int number_of_children = 5;
char buffer[1024] = {0};
char* messsage = "Hello from the server!\r\n";

void signal_handler(int sig) {
    // Wait for all child processes to prevent zombie processes
    while (waitpid(-1, NULL, WNOHANG) > 0);
}

int main(){
    // Signal handler for SIGCHLD to handle terminated child processes
    signal(SIGCHLD, signal_handler);
    // Create a TCP socket
    int listenner = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    // Set up the server address structure
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(8080);
    // Bind socket to server address structure
    if (bind(listenner, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("bind() failed");
        return 1;
    }
    if (listen(listenner, 5) == -1) {
        perror("listen() failed");
        return 1;
    }

    for (int i = 0; i < number_of_children; i++){
        if (fork() == 0){
            // Child process
            while (0 == 0){
                int client = accept(listenner, NULL, NULL);
                if (client == -1){
                    perror("accept() failed");
                    continue;
                } 
                int byte_received = recv(client, buffer, sizeof(buffer), 0);
                printf("Received %d bytes: %s\n", byte_received, buffer);
                if (byte_received == -1){  
                    perror("recv() failed");
                    continue;
                }
                if (send(client, messsage, strlen(messsage), 0) == -1){
                    perror("send() failed");
                    continue;
                }
                printf("Sent message to client: %s\n", messsage);
                close(client);
            }
            exit(0);
        }
    }
    wait(NULL); // Wait for child processes to finish
    return 0;
}