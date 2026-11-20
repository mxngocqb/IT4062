#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>

typedef struct client_list{
    int client;
    char nickname[512];
    int joined; // 0 for not joined, 1 for joined
    struct client_list *next;
} client_list;


client_list *client_head = NULL;
void add_client(int client_socket);
void remove_client(int client_socket);
void join_room(int client_socket, const char *nickname);
char *get_nickname(int client_socket);
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
void *handle_client(void *arg);
int main(){
    // Create a socket
    int server_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(8080);
    server_addr.sin_addr.s_addr = INADDR_ANY;
   if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("bind");
        exit(1);
    }
    listen(server_socket, 5);
    printf("Server is listening on port 8080...\n");

    while (1) {
        // Accept a client connection
        int client_socket = accept(server_socket, NULL, NULL);
        pthread_mutex_lock(&lock);
        add_client(client_socket);
        pthread_mutex_unlock(&lock);
        pthread_t thread;
        pthread_create(&thread, NULL, handle_client, &client_socket);
        pthread_detach(thread);
    }
    return 0;
}

void *handle_client(void *arg){
    int client_socket = *(int *)arg;
    char buffer[1024];
    char response[1024];
    while(0 == 0){
        int bytes_received = recv(client_socket, buffer, sizeof(buffer), 0);
        if(bytes_received <= 0){
            printf("Client disconnected or error occurred.\n");
            break;
        }
        if (strncmp(buffer, "JOIN ", 5) == 0) {
            char *nickname = buffer + 5;
            nickname[strcspn(nickname, "\r\n")] = 0; // Remove newline characters
            join_room(client_socket, nickname);
            snprintf(response, sizeof(response), "Welcome %s!\n", nickname);
            send(client_socket, response, strlen(response), 0);
        } else if (strncmp(buffer, "MSG ", 4) == 0) {
            // send message to all clients
            char *message = buffer + 4;
            printf("Received message from client %d: %s\n", client_socket, message);
            
            client_list *copy;
            pthread_mutex_lock(&lock);
            memcpy(&copy, &client_head, sizeof(client_list *));
            pthread_mutex_unlock(&lock);

            memset(response, 0, sizeof(response));
            snprintf(response, sizeof(response), "%s: %s", get_nickname(client_socket), message);
            
            while(copy != NULL){
                if(copy->client != client_socket && copy->joined == 1){
                    send(copy->client, response, strlen(response), 0);
                }
                copy = copy->next;
            }

        } else if (strncmp(buffer, "PMSG ", 5) == 0) {
            char *message = buffer + 5;

            // Lấy nickname
            char *nickname = strtok(message, " ");

            // Lấy toàn bộ nội dung tin nhắn còn lại
            char *content = strtok(NULL, "");

            
            client_list *copy;
            pthread_mutex_lock(&lock);
            memcpy(&copy, &client_head, sizeof(client_list *));
            pthread_mutex_unlock(&lock);

            memset(response, 0, sizeof(response));
            snprintf(response, sizeof(response), "%s: %s", get_nickname(client_socket), content);
            
            while(copy != NULL){
                if(copy->client != client_socket && copy->joined == 1 && strncmp(copy->nickname, nickname, strlen(copy->nickname)) == 0){
                    send(copy->client, response, strlen(response), 0);
                }
                copy = copy->next;
            }
        } else if (strncmp(buffer, "QUIT", 4) == 0) {
            remove_client(client_socket);
            snprintf(response, sizeof(response), "Goodbye %s!\n", get_nickname(client_socket));
            send(client_socket, response, strlen(response), 0);
            close(client_socket);  
            break;
        } else {
            snprintf(response, sizeof(response), "Unknown command: %s\n", buffer);
            send(client_socket, response, strlen(response), 0);
        }
    }

    pthread_mutex_lock(&lock);
    remove_client(client_socket);
    pthread_mutex_unlock(&lock);

    close(client_socket);
    
    return NULL;
}


void add_client(int client_socket){
    client_list *new_client = (client_list *)malloc(sizeof(client_list));
    new_client->client = client_socket;
    new_client->nickname[0] = '\0';
    new_client->joined = 0;
    new_client->next = client_head;
    
    client_head = new_client;
    while(new_client->next != NULL){
        printf("[%d] -> ", new_client->client);
        new_client = new_client->next;  
    }
    printf("[%d]\n", new_client->client);
}


void remove_client(int client_socket){
    if(client_head == NULL) return;

    if(client_head->client == client_socket){
        client_list *temp = client_head;
        client_head = client_head->next;
        free(temp);
        return;
    }

    client_list *current = client_head;
    while(current->next != NULL && current->next->client != client_socket){
        current = current->next;
    }

    if(current->next != NULL){
        client_list *temp = current->next;
        current->next = current->next->next;
        free(temp);
    }
    
    while(current != NULL){
        printf("[%d] -> ", current->client);
        current = current->next;  
    }
}

void join_room(int client_socket, const char *nickname){
    pthread_mutex_lock(&lock);
    client_list *current = client_head;
    while(current != NULL){
        if(current->client == client_socket){
            strncpy(current->nickname, nickname, sizeof(current->nickname) - 1);
            current->nickname[sizeof(current->nickname) - 1] = '\0'; // Ensure null-termination
            current->joined = 1;
            break;
        }
        current = current->next;
    }
    pthread_mutex_unlock(&lock);
}

char* get_nickname(int client_socket){
    pthread_mutex_lock(&lock);
    client_list *current = client_head;
    while(current != NULL){
        if(current->client == client_socket){
            pthread_mutex_unlock(&lock);
            return current->nickname;
        }
        current = current->next;
    }
    pthread_mutex_unlock(&lock);
    return NULL; // Return NULL if the client is not found
}