#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <signal.h>
#define PORT 8080

void signal_handler(int signum) {
    int status;
    int pid = wait(&status);
    printf("Child process with PID %d terminated with status: %d\n", pid, WEXITSTATUS(status));
}

int main(){
    // Register signal handler for SIGCHLD
    signal(SIGCHLD, signal_handler);
    // Create socket
    int listener = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    // Socket address structure
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(PORT);
    // bind socket to address
    bind(listener, (struct sockaddr*)&addr, sizeof(addr));
    // listen for incoming connections
    listen(listener, 5);

    printf("Server listening on port %d\n", PORT);
    printf("Server PID: %d\n", getpid());

    while (0 == 0){
        int client = accept(listener, NULL, NULL);
        if (fork() == 0){
            // Child process
            close(listener); 
            char buffer[1024] = {0};
            while (0 == 0){
                int bytes_recved = recv(client, buffer, sizeof(buffer), 0);
                printf("Received: %s\n", buffer);
                if (bytes_recved <= 0){
                    break;
                }
                const char *response = "Hello from server!";
                send(client, response, strlen(response), 0);
            }
            exit(0);
        }
        // Parent process
        close(client);
    }
}