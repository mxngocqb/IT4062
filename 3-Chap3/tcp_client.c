#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netdb.h>
#include <string.h>
#include <errno.h>
#include <arpa/inet.h>

int main(){
    int client_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(8080);
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    int res = connect(client_socket, (struct sockaddr*)&addr, sizeof(addr));
    if (res == -1) {
        perror("connect");
        close(client_socket);
        exit(EXIT_FAILURE);     
    }


    char *message = "Hello, Server!";
    ssize_t bytes_sent = send(client_socket, message, strlen(message), 0);
    if (bytes_sent == -1) {
        perror("send");
        close(client_socket);
        exit(EXIT_FAILURE);
    }

    char buffer[1024];
    ssize_t bytes_received = recv(client_socket, buffer, sizeof(buffer) - 1, 0);
    if (bytes_received == -1) {
        perror("recv");
        close(client_socket);
        exit(EXIT_FAILURE);
    }
    buffer[bytes_received] = '\0';
    printf("Received: %s\n", buffer);

    close(client_socket);
    return 0;   
}