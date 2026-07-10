#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>

#define PORT 8080

void *client_handler(void *socket_desc)
{
    int client_sock = *(int *)socket_desc;
    char buffer[1024];

    memset(buffer, 0, sizeof(buffer));

    read(client_sock, buffer, sizeof(buffer));

    printf("Client: %s\n", buffer);

    char reply[] = "Hello from Multithreaded Server";

    write(client_sock, reply, strlen(reply));

    close(client_sock);

    free(socket_desc);

    pthread_exit(NULL);
}

int main()
{
    int server_sock, client_sock;
    struct sockaddr_in server, client;
    socklen_t client_len = sizeof(client);

    pthread_t thread_id;

    server_sock = socket(AF_INET, SOCK_STREAM, 0);

    if(server_sock == -1)
    {
        printf("Socket creation failed\n");
        return 1;
    }

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(PORT);

    bind(server_sock, (struct sockaddr *)&server, sizeof(server));

    listen(server_sock, 5);

    printf("Multithreaded Server Running...\n");

    while(1)
    {
        client_sock = accept(server_sock,
                             (struct sockaddr *)&client,
                             &client_len);

        if(client_sock < 0)
        {
            printf("Accept failed\n");
            continue;
        }

        int *new_sock = malloc(sizeof(int));
        *new_sock = client_sock;

        if(pthread_create(&thread_id,
                          NULL,
                          client_handler,
                          (void *)new_sock) < 0)
        {
            printf("Thread creation failed\n");
            free(new_sock);
            continue;
        }

        pthread_detach(thread_id);
    }

    close(server_sock);

    return 0;
}