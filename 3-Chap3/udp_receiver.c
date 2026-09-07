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
    addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(sender_socket, (struct sockaddr *)&addr, sizeof(addr)) == -1)
    {
        perror("bind");
        close(sender_socket);
        exit(EXIT_FAILURE);
    }

    char buffer[1024];
    while (1)
    {
        int ret = recvfrom(sender_socket, buffer, sizeof(buffer) - 1, 0, NULL, NULL);
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
        }
    }
}