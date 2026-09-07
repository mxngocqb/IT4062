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
    int sender_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(9090);
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    char buffer[1024];
    while (1){
        printf("Enter message to send: ");
        fgets(buffer, sizeof(buffer), stdin);
        buffer[strcspn(buffer, "\n")] = 0; // Remove newline character

        ssize_t bytes_sent = sendto(sender_socket, buffer, strlen(buffer), 0, (struct sockaddr *)&addr, sizeof(addr));
        if (bytes_sent == -1)
        {
            perror("sendto");
            close(sender_socket);
            exit(EXIT_FAILURE);
        }
    }
}