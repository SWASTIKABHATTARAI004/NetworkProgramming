#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/wait.h>

#define PORT 8080

int main()
{
    int server_fd, client_fd;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_len;
    char buffer[1024];
    pid_t pid;

    server_fd = socket(AF_INET, SOCK_STREAM, 0);

    if(server_fd < 0)
    {
        perror("Socket creation failed");
        exit(1);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    if(bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
    {
        perror("Bind failed");
        exit(1);
    }

    listen(server_fd, 5);

    printf("Concurrent Server Running...\n");

    while(1)
    {
        addr_len = sizeof(client_addr);

        client_fd = accept(server_fd,
                           (struct sockaddr *)&client_addr,
                           &addr_len);

        if(client_fd < 0)
        {
            perror("Accept failed");
            continue;
        }

        pid = fork();

        if(pid == 0)
        {
            close(server_fd);

            read(client_fd, buffer, sizeof(buffer));
            printf("Client: %s\n", buffer);

            char reply[] = "Hello from Concurrent Server";
            write(client_fd, reply, strlen(reply));

            close(client_fd);
            exit(0);
        }
        else if(pid > 0)
        {
            close(client_fd);

            while(waitpid(-1, NULL, WNOHANG) > 0);
        }
        else
        {
            perror("Fork failed");
        }
    }

    close(server_fd);

    return 0;
}