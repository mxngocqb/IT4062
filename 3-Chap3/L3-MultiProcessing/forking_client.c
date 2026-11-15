#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "unistd.h"
#include "sys/socket.h"
#include "arpa/inet.h"
#include "signal.h"

int main(){
    // create a TCP socket
    int client = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    // set up the server address structure
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    server_addr.sin_port = htons(8080);
    // connect to the server
    if (connect(client, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1){
        perror("connect() failed");
        return 1;
    }
    char *message = "Hello from the client!\r\n";
    char buffer[1024] = {0};

    if (fork() == 0){
        while (0 == 0){
            fgets(buffer, sizeof(buffer), stdin);
            if (send(client, buffer, strlen(buffer), 0) == -1){
                perror("send() failed");
            }
        }
    } else{
        while (0 == 0){
            int byte_received = recv(client, buffer, sizeof(buffer), 0);
            if (byte_received == -1){
                perror("recv() failed");
                continue;
            }
            if (byte_received == 0){
                printf("Server closed the connection\n");
                break;
            }
            printf("Received %d bytes: %s\n", byte_received, buffer);
        }
    }

    close(client);
    killpg(0, SIGTERM);
    

}