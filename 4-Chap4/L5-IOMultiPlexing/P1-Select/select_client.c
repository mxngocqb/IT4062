#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>

int main(){
    int server_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(8080);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    connect(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr));
    char buffer[1024] = {0};

    fd_set fdread;

    while (1){
        FD_ZERO(&fdread);
        FD_SET(server_socket, &fdread);
        FD_SET(STDIN_FILENO, &fdread);

        int ret = select(server_socket + 1, &fdread, NULL, NULL, NULL);

        if (ret < 0){
            perror("select error");
            exit(1);
        }

        if (FD_ISSET(server_socket, &fdread)){
           memset(buffer, 0, sizeof(buffer));
           int len = recv(server_socket, buffer, sizeof(buffer), 0);
           if (len <= 0){
               printf("Server closed connection\n");
               break;
           }
           printf("Received from server: %s\n", buffer);
        }

        if (FD_ISSET(STDIN_FILENO, &fdread)){
            memset(buffer, 0, sizeof(buffer));
            fgets(buffer, sizeof(buffer), stdin);
            send(server_socket, buffer, strlen(buffer), 0);
        }


    }

}