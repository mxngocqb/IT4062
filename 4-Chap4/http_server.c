#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/wait.h>
#include <pthread.h>
#include <unistd.h>
#include <malloc.h>
#include <dirent.h>

typedef struct sockaddr SOCKADDR;
typedef struct sockaddr_in SOCKADDR_IN;
typedef struct in_addr IN_ADDR;

const char* rootFolder = "/";
char currentPath[8192] = { 0 };

void AppendStr(char** output, const char* str)
{
    char* tmp = *output;
    int oldlen = tmp == NULL ? 0 : strlen(tmp);
    int newlen = oldlen + strlen(str) + 1;
    tmp = realloc(tmp, newlen);
    tmp[newlen - 1] = 0;
    sprintf(tmp + oldlen, "%s", str);
    *output = tmp;
}

void AppendInt(char** output, const int d)
{
    char str[1024] = { 0 };
    sprintf(str, "%d", d);
    char* tmp = *output;
    int oldlen = tmp == NULL ? 0 : strlen(tmp);
    int newlen = oldlen + strlen(str) + 1;
    tmp = realloc(tmp, newlen);
    tmp[newlen - 1] = 0;
    sprintf(tmp + oldlen, "%s", str);
    *output = tmp;
}

int dirsort(const struct dirent ** x, const struct dirent ** y)
{
    const struct dirent* a = *x;
    const struct dirent* b = *y;
    if (a->d_type == b->d_type)
    {
        return -strcmp(a->d_name, b->d_name);
    }else if (a->d_type == DT_DIR)
        return 1;
    else
        return -1;    
}


void* ClientThread(void* param)
{
    int c = *((int*)param);
    char buffer[1024 * 1024] = { 0 };
    int len = 0;
    while (strstr(buffer,"\r\n\r\n") == NULL)
    {
        int r = recv(c, buffer + len, sizeof(buffer) - len - 1, 0);
        if (r <= 0)
        {
            break;
        }else
            len += r;
    }
    if (strstr(buffer,"\r\n\r\n") != NULL)
    {
        char method[1024] = { 0 };
        char path[2048] = { 0 };
        sscanf(buffer, "%s%s", method, path);
        while (strstr(path,"%20"))
        {
            char* found = strstr(path,"%20");
            char prev[1024] = { 0 };
            char post[1024] = { 0 };
            memcpy(prev, path, found - path);
            strcpy(post, found + 3);
            memset(path, 0, sizeof(path));
            sprintf(path, "%s %s", prev, post);
        }
        if (strcmp(method,"GET") == 0)
        {
            if (strcmp(path,"/") == 0)
            {
                strcpy(currentPath,"/");
                char* content = NULL;
                char* header = NULL;

                AppendStr(&content,"<html>");
                AppendStr(&content,"<form action=\"/mnt/g/WSL\" method=\"post\" enctype=\"multipart/form-data\">");
                AppendStr(&content,"Select image to upload:<br>");
                AppendStr(&content, "<input type=\"file\" name=\"file1\" id=\"file1\"><br>");
                AppendStr(&content, "<input type=\"file\" name=\"file2\" id=\"file2\"><br>");
                AppendStr(&content,"<input type=\"submit\" value=\"Upload\" name=\"submit\">");
                AppendStr(&content,"</form><br>");
                struct dirent **namelist;
                int n = scandir(rootFolder, &namelist, NULL, dirsort);
                if (n > 0)
                {
                    while (n--) {
                        if (namelist[n]->d_type == DT_DIR)
                        {
                            AppendStr(&content, "<a href=\"");
                            AppendStr(&content,currentPath);
                            AppendStr(&content,namelist[n]->d_name);
                            AppendStr(&content,"\">");
                            AppendStr(&content,"<b>");
                            AppendStr(&content, namelist[n]->d_name);
                            AppendStr(&content,"</b></a><br>");
                        }else
                        {
                            AppendStr(&content,"<a href=\"\">");
                            AppendStr(&content,"<i>");
                            AppendStr(&content, namelist[n]->d_name);
                            AppendStr(&content,"</i></a><br>");
                        }
                        free(namelist[n]);
                        namelist[n] = NULL;
                    }
                    free(namelist);
                    namelist = NULL;
                }else
                {
                    AppendStr(&content, "<h>FOLDER EMPTY</h><br>");
                }
                AppendStr(&content,"</html>");

                AppendStr(&header, "HTTP/1.1 200 OK\r\nContent-Length: ");
                AppendInt(&header, strlen(content));
                AppendStr(&header,"\r\n\r\n");

                send(c, header, strlen(header), 0);
                send(c, content, strlen(content), 0);

                free(header);
                header = NULL;
                free(content);
                content = NULL;
            }else
            {
                if (path[strlen(path) - 1] != '*')
                {
                    if (strstr(path,"favicon.ico"))
                    {
                        FILE* f = fopen("favicon.ico", "rb");
                        if (f != NULL)
                        {
                            fseek(f,0,SEEK_END);
                            int flen = ftell(f);
                            fseek(f, 0, SEEK_SET);

                            char* header = NULL;
                            AppendStr(&header, "HTTP/1.1 200 OK\r\nContent-Length:");
                            AppendInt(&header, flen);
                            AppendStr(&header,"\r\n\r\n");
                            send(c, header, strlen(header), 0);
                            free(header);
                            header = NULL;

                            char buffer[1024] =  { 0 };
                            int read = 0;
                            while (read < flen)
                            {
                                int r = fread(buffer, 1, sizeof(buffer), f);
                                read += r;
                                send(c, buffer, r, 0);
                            }
                            fclose(f);
                        }else
                        {
                            char* header = NULL;
                            AppendStr(&header, "HTTP/1.1 500 Internal Server Error\r\n\r\n");
                            send(c, header, strlen(header), 0);
                            free(header);
                            header = NULL;
                        }
                    }else
                    {
                        strcpy(currentPath, path);
                        char* content = NULL;
                        char* header = NULL;

                        AppendStr(&content,"<html>");
                        struct dirent **namelist;
                        int n = scandir(currentPath, &namelist, NULL, dirsort);
                        if (n > 0)
                        {
                            while (n--) {
                                if (namelist[n]->d_type == DT_DIR)
                                {
                                    AppendStr(&content, "<a href=\"");
                                    AppendStr(&content,currentPath);
                                    if (currentPath[strlen(currentPath) - 1] != '/')
                                    {
                                        AppendStr(&content,"/");
                                    }
                                    AppendStr(&content,namelist[n]->d_name);
                                    AppendStr(&content,"\">");
                                    AppendStr(&content,"<b>");
                                    AppendStr(&content, namelist[n]->d_name);
                                    AppendStr(&content,"</b></a><br>");
                                }else
                                {
                                    AppendStr(&content, "<a href=\"");
                                    AppendStr(&content,currentPath);
                                    if (currentPath[strlen(currentPath) - 1] != '/')
                                    {
                                        AppendStr(&content,"/");
                                    }
                                    AppendStr(&content,namelist[n]->d_name);
                                    AppendStr(&content,"*");
                                    AppendStr(&content,"\">");
                                    AppendStr(&content,"<i>");
                                    AppendStr(&content, namelist[n]->d_name);
                                    AppendStr(&content,"</i></a><br>");
                                }
                                free(namelist[n]);
                                namelist[n] = NULL;
                            }
                            free(namelist);
                            namelist = NULL;
                        }else
                        {
                            AppendStr(&content, "<h>FOLDER EMPTY</h><br>");
                        }
                        AppendStr(&content,"</html>");

                        AppendStr(&header, "HTTP/1.1 200 OK\r\nContent-Length: ");
                        AppendInt(&header, strlen(content));
                        AppendStr(&header,"\r\n\r\n");

                        send(c, header, strlen(header), 0);
                        send(c, content, strlen(content), 0);

                        free(header);
                        header = NULL;
                        free(content);
                        content = NULL;
                    }
                }else
                {
                    path[strlen(path) - 1] = 0;
                    FILE* f = fopen(path,"rb");
                    if (f != NULL)
                    {
                        fseek(f, 0, SEEK_END);
                        int filesize = ftell(f);
                        fseek(f, 0, SEEK_SET);
                        char* header = NULL;
                        char content[1024] = { 0 };
                        AppendStr(&header, "HTTP/1.1 200 OK\r\nContent-Type: ");
                        if (strncmp(path + strlen(path) - 4,".png", 4) == 0)
                        {
                            AppendStr(&header, "image/png\r\n");
                        }else if (strncmp(path + strlen(path) - 4,".pdf", 4) == 0)
                        {
                            AppendStr(&header, "application/pdf\r\n");
                        }else if (strncmp(path + strlen(path) - 4,".txt", 4) == 0)
                        {
                            AppendStr(&header, "text/plain\r\n");
                        }else
                        {
                            AppendStr(&header, "application/octet-stream\r\n");
                        }
                        AppendStr(&header,"Content-Length: ");    
                        AppendInt(&header, filesize);
                        AppendStr(&header,"\r\n\r\n");

                        send(c, header, strlen(header), 0);
                        int sent = 0;
                        while (sent < filesize)
                        {
                            int r = fread(content, 1, sizeof(content), f);
                            send(c, content, r, 0);
                            sent += r;
                        }

                        fclose(f);
                        free(header);
                        header = NULL;
                    }else
                    {
                        char* header = NULL;
                        AppendStr(&header, "HTTP/1.1 500 Internal Server Error\r\n\r\n");
                        send(c, header, strlen(header), 0);
                        free(header);
                        header = NULL;
                    }
                }   
            }
        }else if (strcmp(method,"POST") == 0)
        {
            char* str = strstr(buffer,"Content-Length");
            char text[1024] = { 0 };
            int ctlen = 0;
            sscanf(str,"%s%d", text, &ctlen);

            str = strstr(buffer, "Content-Type: multipart/form-data; boundary=");
            str += strlen("Content-Type: multipart/form-data; boundary=");
            char boundary[1024] = { 0 };
            sscanf(str,"%s", boundary);

            int received = 0;
            char* data = (char*)calloc(1, ctlen);
            while (received < ctlen)
            {
                int r = recv(c, data + received, ctlen - received, 0);
                if (r <= 0 )
                {
                    break;
                }else
                    received += r;
            }
            
            str = strstr(data, "filename=");
            while (str != NULL)
            {
                char* start = str + 10;
                char* stop = strstr(start, "\"") - 1;
                char filename[1024] = { 0 };
                memcpy(filename, start, stop - start + 1);
                FILE* f = fopen(filename,"wb");
                start = strstr(start,"\r\n\r\n") + 4;
                char* temp = start;
                stop = strstr(temp, boundary);
                while (stop == NULL)
                {
                    temp++;
                    stop = strstr(temp, boundary);
                }
                stop -= 5;
                fwrite(start, 1, stop - start + 1, f);
                fclose(f);
                str = strstr(stop, "filename=");
            }

            char* header = NULL;
            AppendStr(&header, "HTTP/1.1 200 OK\r\n\r\n");
            send(c, header, strlen(header), 0);
            free(header);
            header = NULL;
        }
    }
    close(c);
}

int main()
{
    int s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    SOCKADDR_IN saddr, caddr;
    saddr.sin_family = AF_INET;
    saddr.sin_port = htons(8888);
    saddr.sin_addr.s_addr = inet_addr("0.0.0.0");
    if (bind(s, (SOCKADDR*)&saddr, sizeof(saddr)) == 0)
    {
        listen(s, 10);
        while (0 == 0)
        {
            int clen = sizeof(caddr);
            printf("Waiting...\n");
            int c = accept(s, (SOCKADDR*)&caddr, &clen);
            if (c >= 0)
            {
                printf("A client has connected\n");
                pthread_t pid;
                int* param = (int*)calloc(1, sizeof(int));
                *param = c;
                pthread_create(&pid, NULL, ClientThread, (void*)param);
            }else
                break;
        }
    }else
        printf("Failed to bind!\n");
}
