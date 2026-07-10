#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define PORT 8080

int server_fd;

void signal_handler(int sig)
{
    printf("\nSIGINT received.\n");
    printf("Closing server socket...\n");

    close(server_fd);

    printf("Server terminated successfully.\n");

    exit(0);
}

int main()
{
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_len;
    int client_fd;
    char buffer[1024];

    signal(SIGINT, signal_handler);

    server_fd = socket(AF_INET, SOCK_STREAM, 0);

    if(server_fd < 0)
    {
        perror("Socket creation failed");
        return 1;
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr));

    listen(server_fd, 5);

    printf("Server is waiting for client...\n");

    addr_len = sizeof(client_addr);

    client_fd = accept(server_fd,
                       (struct sockaddr *)&client_addr,
                       &addr_len);

    read(client_fd, buffer, sizeof(buffer));

    printf("Client: %s\n", buffer);

    write(client_fd,
          "Hello from Server",
          strlen("Hello from Server"));

    close(client_fd);
    close(server_fd);

    return 0;
}