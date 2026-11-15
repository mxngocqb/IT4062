#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netdb.h>
#include <string.h>
#include <errno.h>
#include <arpa/inet.h>
#include <poll.h>

int main()
{
    // Tạo Socket UDP
    int sender_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

    // Thiết lập địa chỉ và cổng cho socket
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(9090);
    addr.sin_addr.s_addr = INADDR_ANY;
    // Thiết lập cấu trúc lưu đỉa socket của client
    struct sockaddr_in client_socket;
    socklen_t client_socket_len = sizeof(client_socket);

    // Liên kết socket với địa chỉ và cổng
    if (bind(sender_socket, (struct sockaddr *)&addr, sizeof(addr)) == -1)
    {
        perror("bind");
        close(sender_socket);
        exit(EXIT_FAILURE);
    }

    printf("UDP Receiver is listening on port %d...\n", ntohs(addr.sin_port));

    char buffer[1024];
    while (1)
    {
        // Nhận dữ liệu từ client
        int ret = recvfrom(sender_socket, buffer, sizeof(buffer) - 1, 0, (struct sockaddr *)&client_socket, &client_socket_len);
        if (ret == -1)
        {
            perror("recvfrom");
            close(sender_socket);
            exit(EXIT_FAILURE);
        }
        else
        {
            buffer[ret] = '\0';
            printf("Received: %s\n", buffer);
            printf("Client IP: %s, Port: %d\n, Family: %d\n", inet_ntoa(client_socket.sin_addr), ntohs(client_socket.sin_port), client_socket.sin_family);
            // Gửi phản hồi lại cho client
            char response[1024];
            sprintf(response, "Tao đã nhận được %d bytes dữ liệu", ret);
            ssize_t bytes_sent = sendto(
                sender_socket, 
                response, 
                strlen(response), 
                0, 
                (struct sockaddr *)&client_socket, 
                client_socket_len
            );
            if (bytes_sent == -1)
            {
                perror("sendto");
                close(sender_socket);
                exit(EXIT_FAILURE);    
            }
        }
    }
}