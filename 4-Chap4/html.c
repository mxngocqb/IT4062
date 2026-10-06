#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
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

char *BuildDirectoryHTML(const char *path)
{
    DIR *dir = opendir(path);

    if (dir == NULL)
    {
        return NULL;
    }

    char *html = NULL;

    struct dirent *entry;

    while ((entry = readdir(dir)) != NULL)
    {
        char line[1024];

        if (entry->d_type == DT_DIR)
        {
            sprintf(
                line,
                "<a href=\"%s/\">[DIR] %s</a><br>",
                entry->d_name,
                entry->d_name);
        }
        else
        {
            sprintf(
                line,
                "<a href=\"%s\?download=1\" download>%s</a><br>",
                entry->d_name,
                entry->d_name);
        }

        Append(&html, line);
    }

    closedir(dir);

    return html;
}

void ParseHTTPRequest(
    const char *buffer,
    char *method,
    char *endpoint)
{
    sscanf(buffer, "%15s %1023s", method, endpoint);
}

int main()
{
    int listener = socket(
        AF_INET,
        SOCK_STREAM,
        IPPROTO_TCP);

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(8080);
    addr.sin_addr.s_addr = INADDR_ANY;

    int opt = 1;
    setsockopt(listener, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    if (bind(
            listener,
            (struct sockaddr *)&addr,
            sizeof(addr)) == -1)
    {
        perror("bind() failed");
        return 1;
    }

    if (listen(listener, 5) == -1)
    {
        perror("listen() failed");
        return 1;
    }

    char buffer[1024] = {0};
    char method[16] = {0};
    char endpoint[1024] = {0};

    while (0 == 0)
    {
        int client = accept(
            listener,
            NULL,
            NULL);

        if (client == -1)
        {
            perror("accept() failed");
            return 1;
        }

        memset(buffer, 0, sizeof(buffer));
        memset(method, 0, sizeof(method));
        memset(endpoint, 0, sizeof(endpoint));
        int byte_received = recv(
            client,
            buffer,
            sizeof(buffer),
            0);

        if (byte_received == -1)
        {
            perror("recv() failed");
            return 1;
        }

        ParseHTTPRequest(buffer, method, endpoint);

        printf("Received request: %s %s\n", method, endpoint);
        if (strstr(endpoint, "?download=1") != NULL)
        {
            char *query = strstr(endpoint, "?download=1");
            *query = '\0';

            // Bỏ dấu '/' đầu tiên
            char *file_path = endpoint;

            FILE *file = fopen(file_path, "rb");

            if (file == NULL)
            {
                perror("fopen() failed");

                const char *response =
                    "HTTP/1.1 404 Not Found\r\n"
                    "Content-Length: 0\r\n"
                    "\r\n";

                send(client, response, strlen(response), 0);

                close(client);
                continue;
            }

            // Lấy kích thước file
            fseek(file, 0, SEEK_END);
            long file_size = ftell(file);
            rewind(file);

            // Lấy tên file
            char *filename = strrchr(file_path, '/');

            if (filename == NULL)
                filename = file_path;
            else
                filename++;

            // Gửi HTTP header trước
            char header[1024];

            sprintf(
                header,
                "HTTP/1.1 200 OK\r\n"
                "Content-Type: application/octet-stream\r\n"
                "Content-Disposition: attachment; filename=\"%s\"\r\n"
                "Content-Length: %ld\r\n"
                "\r\n",
                filename,
                file_size);

            send(client, header, strlen(header), 0);

            // Gửi nội dung file
            char file_buffer[1024];
            size_t bytes_read;

            while ((bytes_read = fread(
                        file_buffer,
                        1,
                        sizeof(file_buffer),
                        file)) > 0)
            {
                send(client, file_buffer, bytes_read, 0);
            }

            fclose(file);

            close(client);
            continue;
        }
        else
        {
            content = NULL;

            Append(&content, "HTTP/1.1 200 OK\r\n");
            Append(&content, "Content-Type: text/html\r\n");
            Append(&content, "\r\n");
            Append(&content, "<h1>Show server directory</h1>");
            Append(&content, "<p>This is a simple HTML page.</p>");
            char *directory_html = BuildDirectoryHTML(endpoint); // Skip the leading '/'
            if (directory_html != NULL)
            {
                Append(&content, directory_html);
                free(directory_html);
            }

            printf("%s", directory_html);

            send(
                client,
                content,
                strlen(content),
                0);
        }

        close(client);
    }
}