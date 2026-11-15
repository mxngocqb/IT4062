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
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    
    char buffer[1024];
    while (1){
        printf("Enter message to send: ");
        memset(buffer, 0, sizeof(buffer));
        fgets(buffer, sizeof(buffer), stdin);
        buffer[strcspn(buffer, "\n")] = 0; // Remove newline character
        // Gửi dữ liệu đến server
        ssize_t bytes_sent = sendto(sender_socket, buffer, strlen(buffer), 0, (struct sockaddr *)&addr, sizeof(addr));
    
        if (bytes_sent == -1)
        {
            perror("sendto");
            close(sender_socket);
            exit(EXIT_FAILURE);
        } else if (strcmp(buffer, "GETLIST") == 0){
             // Nhận dữ liệu phản hồi từ server
            while (0==0) {
                memset(buffer, 0, sizeof(buffer));
                struct sockaddr_in server_addr;
                socklen_t server_addr_len = sizeof(server_addr);
                ssize_t bytes_received = recvfrom(sender_socket, buffer, sizeof(buffer) - 1, 0, (struct sockaddr *)&server_addr, &server_addr_len);
                if (bytes_received == -1)
                {
                    perror("recvfrom");
                    close(sender_socket);
                    exit(EXIT_FAILURE);
                }
                // Xử lý dữ liệu nhận được
                buffer[bytes_received] = '\0';
                printf("Received: %s\n", buffer);
                if (strcmp(buffer, "END_OF_LIST") == 0) {
                    break; // End of list
                }
            }
        } else if (strncmp(buffer, "GETSONG ", 8) == 0){
            char *song_name = buffer + 8;
            FILE *file = fopen(song_name, "wb");
            if (file == NULL) {
                perror("fopen");
                continue;
            }  

            while (0==0) {
                memset(buffer, 0, sizeof(buffer));
                struct sockaddr_in server_addr;
                socklen_t server_addr_len = sizeof(server_addr);
                ssize_t bytes_received = recvfrom(sender_socket, buffer, sizeof(buffer), 0, (struct sockaddr *)&server_addr, &server_addr_len);
                if (bytes_received == -1) {
                    perror("recvfrom");
                    fclose(file);
                    close(sender_socket);
                    exit(EXIT_FAILURE);
                }
                if (bytes_received == 0) {
                    break; // End of file
                }
                fwrite(buffer, 1, bytes_received, file);
                if (strcmp(buffer, "END_OF_FILE") == 0) {
                    break; // End of file
                }   
            }
            printf("Download complete: %s\n", song_name);
            fclose(file);
        }
        
    }
}