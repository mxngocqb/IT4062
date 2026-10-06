#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/ioctl.h>

int main(){
    int listener = socket(
        AF_INET, 
        SOCK_STREAM,
        IPPROTO_TCP
    );

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(8080);
    addr.sin_addr.s_addr = INADDR_ANY;

    int opt = 1;
    setsockopt(listener, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));


    if (bind(
            listener, 
            (struct sockaddr*)&addr, 
            sizeof(addr)
        ) == -1
    ){
        perror("bind() failed");
        return 1;
    }

    if (listen(listener, 5) == -1){
        perror("listen() failed");
        return 1;
    }

    char buffer[1024] = {0};

    while (0 == 0){
        int client = accept(
            listener, 
            NULL, 
            NULL
        );

        if (client == -1){
            perror("accept() failed");
            return 1;
        }

        char message[] = "Hello from server!\r\n";
        send(client, message, strlen(message), 0);

        int byte_received = recv(
            client, 
            buffer, 
            sizeof(buffer), 
            0
        );

        if (byte_received == -1){
            perror("recv() failed");
            return 1;
        }

        printf("Received %d bytes: %s\n", byte_received, buffer);
        close(client);
    }

}