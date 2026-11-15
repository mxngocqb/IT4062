#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

#define SOCKADDR_IN struct sockaddr_in 
#define SOCKADDR struct sockaddr 

int main()
{
    int s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    SOCKADDR_IN saddr, caddr;
    int clen = sizeof(caddr);
    saddr.sin_family = AF_INET;
    saddr.sin_port = htons(8888);
    saddr.sin_addr.s_addr = inet_addr("0.0.0.0");
    int error = bind(s, (SOCKADDR*)&saddr, sizeof(saddr));
    if (error < 0)
    {
        printf("Failed to bind to port number 8888\n");
        close(s);
    }else
    {
        listen(s, 10);
        while (0 == 0)
        {
            int c = accept(s, (SOCKADDR*)&caddr, &clen);
            if (c > 0)
            {
                char* welcome = "Simple Telnet Server\n";
                int sent = send(c, welcome, strlen(welcome), 0);
                printf("Sent: %d (Bytes)\n", sent);    
                char buffer[1024] = { 0 };
                int received = recv(c, buffer, sizeof(buffer) - 1, 0);
                while (buffer[strlen(buffer) - 1] == '\r' || buffer[strlen(buffer) - 1] == '\n')
                {
                    buffer[strlen(buffer) - 1] = 0;
                }
                strcat(buffer, "> out.txt");
                system(buffer);

                FILE* f = fopen("out.txt","rt");
                while (!feof(f))
                {
                    memset(buffer, 0, sizeof(buffer));
                    fgets(buffer, sizeof(buffer) - 1, f);
                    send(c, buffer, strlen(buffer), 0);
                }
                send(c, "\n", 1, 0);
                fclose(f);
                close(c);
            }
        }
    }
}