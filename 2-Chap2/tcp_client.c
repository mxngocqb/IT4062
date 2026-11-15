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
    // Tạo Socket TCP
    int client_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    // Tạo địa chỉ và cổng của server mà client muốn kết nối
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(8080);
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    // Kết nối đến server
    int res = connect(client_socket, (struct sockaddr*)&addr, sizeof(addr));
    if (res == -1) {
        perror("connect");
        close(client_socket);
        exit(EXIT_FAILURE);     
    }

    // Gửi dữ liệu đến server
    char *message = "Hello, Server!";
    ssize_t bytes_sent = send(client_socket, message, strlen(message), 0);
    if (bytes_sent == -1) {
        perror("send");
        close(client_socket);
        exit(EXIT_FAILURE);
    }

    // Nhận dữ liệu phản hồi từ server
    char buffer[1024];
    ssize_t bytes_received = recv(client_socket, buffer, sizeof(buffer) - 1, 0);
    if (bytes_received == -1) {
        perror("recv");
        close(client_socket);
        exit(EXIT_FAILURE);
    }
    // Xử lý dữ liệu nhận được 
    buffer[bytes_received] = '\0';
    printf("Received: %s\n", buffer);

    close(client_socket);
    return 0;   
}