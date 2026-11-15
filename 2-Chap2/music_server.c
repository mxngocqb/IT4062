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

#define BLOCK_SIZE 1024
#define DIR_PATH "/Users/ngocmx/HUST_Lecture/IT4062/3-Chap3/server_playlist"

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
        memset(buffer, 0, sizeof(buffer));
        int ret = recvfrom(sender_socket, buffer, sizeof(buffer) - 1, 0, (struct sockaddr *)&client_socket, &client_socket_len);
        if (ret == -1)
        {
            perror("recvfrom");
            close(sender_socket);
            exit(EXIT_FAILURE);
        }
        else
        {
            while (buffer[strlen(buffer) - 1] == '\r' || buffer[strlen(buffer) - 1] == '\n'){
                buffer[strlen(buffer) - 1] = 0;
            }

            if (strcmp(buffer, "GETLIST") == 0){
                system("ls " DIR_PATH " > list.txt");
                FILE *file = fopen("list.txt", "r");
                if (file == NULL){
                    sendto(sender_socket, "Error opening list.txt", strlen("Error opening list.txt"), 0, (struct sockaddr *)&client_socket, client_socket_len);
                    continue;
                } else{
                    char line[1024];
                    while (fgets(line, sizeof(line), file) != NULL){
                        // Gửi từng dòng trong list.txt đến client
                        ssize_t bytes_sent = sendto(sender_socket, line, strlen(line), 0, (struct sockaddr *)&client_socket, client_socket_len);
                        if (bytes_sent == -1){
                            perror("sendto");
                            close(sender_socket);
                            exit(EXIT_FAILURE);
                        }
                    }
                    char end_of_list_msg[] = "END_OF_LIST";
                    sendto(sender_socket, end_of_list_msg, strlen(end_of_list_msg), 0, (struct sockaddr *)&client_socket, client_socket_len);           
                    fclose(file);
                }
            } else if (strncmp(buffer, "GETSONG ", 8) == 0) {
                char *song_name = buffer + 8;
                printf("Song name: %s\n", song_name);
                char* data = calloc(BLOCK_SIZE, 1);
                char song_path[1024];
                snprintf(song_path, sizeof(song_path), "%s/%s", DIR_PATH, song_name);
                FILE *file = fopen(song_path, "rb");
                if (file == NULL){
                    perror("fopen");
                    char error_msg[1024];
                    snprintf(error_msg, sizeof(error_msg), "Error opening file: %s", strerror(errno));
                    sendto(sender_socket, error_msg, strlen(error_msg), 0, (struct sockaddr *)&client_socket, client_socket_len);
                    continue;
                }
                size_t bytes_read;
                while ((bytes_read = fread(data, 1, BLOCK_SIZE, file)) > 0) {
                    ssize_t bytes_sent = sendto(sender_socket, data, bytes_read, 0, (struct sockaddr *)&client_socket, client_socket_len);
                    if (bytes_sent == -1) {
                        perror("sendto");
                        fclose(file);
                        close(sender_socket);
                        exit(EXIT_FAILURE);
                    }
                }
                char end_of_file_msg[] = "END_OF_FILE";
                sendto(sender_socket, end_of_file_msg, strlen(end_of_file_msg), 0, (struct sockaddr *)&client_socket, client_socket_len);
                fclose(file);
                free(data); 
            } else{
                printf("Received unknown command: %s\n", buffer);
                char error_msg[1024];
                snprintf(error_msg, sizeof(error_msg), "Unknown command: %s", buffer);
                sendto(sender_socket, error_msg, strlen(error_msg), 0, (struct sockaddr *)&client_socket, client_socket_len);
            }
        }
    }
}