#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netdb.h>
#include <string.h>
#include <errno.h>
#include <arpa/inet.h>
#include <sys/ioctl.h>

void remove_client(int *clients, int *num_clients, int index)
{
    // Close the client socket
    close(clients[index]);

    // Shift the remaining clients in the array
    for (int i = index; i < (*num_clients) - 1; i++)
    {
        clients[i] = clients[i + 1];
    }
    (*num_clients)--;
}

int main()
{
    // Create a TCP Socket
    int listener_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (listener_socket == -1)
    {
        perror("socket");
        return 1;   
    }

    // Set the socket to non-blocking mode
    unsigned long mode = 1; // 1 to enable non-blocking mode
    if (ioctl(listener_socket, FIONBIO, &mode) == -1)
    {
        perror("ioctl");
        close(listener_socket);
        return 1;   
    }   

    // define the server address
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(9000);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);

    // bind the socket to the address
    if (bind(listener_socket, (struct sockaddr *)&addr, sizeof(addr)) == -1)
    {
        perror("bind");
        close(listener_socket);
        return 1;       
    }

    if (listen(listener_socket, 5) == -1)
    {
        perror("listen");
        close(listener_socket);
        return 1;       
    }

    int clients[50];
    int num_clients = 0;
    char buffer[1024];

    while (1)
    {
        // Accept new connections
        int client_socket = accept(listener_socket, NULL, NULL);
        if (client_socket == -1)
        {
            if (errno != EWOULDBLOCK){
                printf("Accept() failed");
            }else {
                // Add the new client socket to the clients array
            }
        }else{
            // If a new client is accepted, add it to the clients array
            printf("New client connected: %d\n", client_socket);
            clients[num_clients++] = client_socket;
            mode = 1;
            ioctl(client_socket, FIONBIO, &mode);
        }

        // Check for incoming data from connected clients
        for (int i = 0; i < num_clients; i++)
        {
            int ret = recv(clients[i], buffer, sizeof(buffer) - 1, 0);
            if (ret == -1)
            {
                if (errno != EWOULDBLOCK)
                {
                    printf("recv() failed for client %d\n", clients[i]);    
                    close(clients[i]);
                    remove_client(clients, &num_clients, i);
                    continue; // Skip to the next client
                } else {
                    // If data is received, print it
                } 
            }else if (ret == 0){
                printf("Client %d disconnected\n", clients[i]);
                close(clients[i]);
                remove_client(clients, &num_clients, i);
                continue; // Skip to the next client
            } else{
                buffer[ret] = '\0';
                send(clients[i], buffer, ret, 0); // Echo the received data back to the client
                printf("Received from client %d: %s\n", clients[i], buffer);
            }
        }
    }
    close(listener_socket);
    return 0;  
}