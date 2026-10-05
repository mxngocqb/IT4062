#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <dirent.h>

char *content = NULL;

void Append(char **output, const char *str)
{
    char *tmp = *output;
    int oldlen = tmp == NULL ? 0 : strlen(tmp);
    int newlen = oldlen + strlen(str) + 1;
    tmp = realloc(tmp, newlen);
    tmp[newlen - 1] = 0;
    sprintf(tmp + oldlen, "%s", str);
    *output = tmp;
}

int main()
{
    int listener = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (listener == -1)
    {
        perror("socket() failed");
        exit(1);
    }

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(8090);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);

    int opt = 1;
    setsockopt(listener, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    if (bind(listener, (struct sockaddr *)&addr, sizeof(addr)) == -1)
    {
        perror("bind() failed");
        exit(1);
    }

    if (listen(listener, 5) == -1)
    {
        perror("listen() failed");
        exit(1);
    }

    char buffer[1024] = {0};
    printf("Listening on port 8090...\n");
    while (1)
    {
        int client = accept(listener, NULL, NULL);
        memset(buffer, 0, sizeof(buffer));
        ssize_t recv_bytes = recv(client, buffer, sizeof(buffer) - 1, 0);
        if (recv_bytes <= 0)
        {
            close(client);
            continue;
        }
        printf("Received request:\n%s\n", buffer);

        content = NULL;
        Append(&content, "HTTP/1.1 200 OK\r\n");
        Append(&content, "Content-Type: text/html; charset=utf-8\r\n");
        Append(&content, "Connection: close\r\n");
        Append(&content, "\r\n");
        Append(&content, "<h1>Directory folder</h1>");
        Append(&content, "<p>This is a simple directory listing.</p>");

        DIR *dir = opendir(".");

        struct dirent *entry;

        while ((entry = readdir(dir)) != NULL)
        {
           if (entry->d_type == DT_DIR)
            {
                // show directory as link
                Append(&content, "<p>[DIR] <a href=\"");
                Append(&content, entry->d_name);
                Append(&content, "\">");
                Append(&content, entry->d_name);
                Append(&content, "</a></p>");
            }
            else if (entry->d_type == DT_REG)
            {   // show as link
                Append(&content, "<p>[FILE] <a href=\"");
                Append(&content, entry->d_name);
                Append(&content, "\">");
                Append(&content, entry->d_name);
                Append(&content, "</a></p>");
            }
            else
            {
                Append(&content, "<p>[OTHER] ");
                Append(&content, entry->d_name);
                Append(&content, "</p>");
            }
        }

        closedir(dir);

        ssize_t sent_bytes = send(client, content, strlen(content), 0);

        if (sent_bytes == -1)
        {
            perror("send() failed");
        }
        else
        {
            printf("Sent response:\n%s\n", content);
        }
        close(client);
    }
}