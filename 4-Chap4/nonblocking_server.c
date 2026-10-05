#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <sys/types.h>

#define MAX_CLIENTS 10
int socket_mode = 1;

int main()
{
    // khởi tạo socket
    int listener = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (listener == -1)
    {
        perror("socket() failed");
        exit(1);
    }
    // thiết lập địa chỉ server
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(8080);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    if (bind(listener, (struct sockaddr *)&addr, sizeof(addr)) == -1)
    {
        perror("bind() failed");
        exit(1);
    }
    char buffer[1024] = {0};
    char *message = "Hello from server!";
    if (listen(listener, 5) == -1)
    {
        perror("listen() failed");
        exit(1);
    }
    ioctl(listener, FIONBIO, &socket_mode);

    int clients[MAX_CLIENTS] = {-1};
    int num_clients = 0;
    // lắng nghe kết nối
    while (0 == 0)
    {
        int client = accept(listener, NULL, NULL);
        if (num_clients < MAX_CLIENTS && client != -1)
        {
            printf("New client connected: %d\n", client);
            clients[num_clients++] = client;
            ioctl(client, FIONBIO, &socket_mode);
        }
        for (int i = 0; i < num_clients; i++)
        {
            if (clients[i] != -1)
            {
                memset(buffer, 0, sizeof(buffer));
                int bytes_received = recv(clients[i], buffer, sizeof(buffer), 0);
                if (bytes_received > 0)
                {
                    printf("Received from client %d: %s\n", clients[i], buffer);
                    for (int j = 0; j < num_clients; j++)
                    {
                        // Send to all clients except the sender
                        if (clients[i] != -1 && clients[i] != clients[j])
                        {
                            send(clients[j], buffer, bytes_received, 0);
                        }
                    }
                }
                else if (bytes_received == 0)
                {
                    printf("Client %d disconnected\n", clients[i]);
                    close(clients[i]);
                    clients[i] = -1;
                }
            }
        }
    }
}
