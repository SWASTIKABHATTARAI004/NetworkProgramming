#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080

int main()
{
    int server_fd, client_fd;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_len = sizeof(client_addr);
    char buffer[1024] = {0};

    // Create socket
    server_fd = socket(AF_INET, SOCK_STREAM, 0);

    if (server_fd == -1)
    {
        printf("Socket creation failed!\n");
        return 1;
    }

    // Server configuration
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    // Bind socket
    if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
    {
        printf("Bind failed!\n");
        return 1;
    }

    // Listen for clients
    listen(server_fd, 5);

    printf("Server is waiting for connection...\n");

    // Accept client
    client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &addr_len);

    if (client_fd < 0)
    {
        printf("Accept failed!\n");
        return 1;
    }

    // Receive message
    read(client_fd, buffer, sizeof(buffer));
    printf("Client: %s\n", buffer);

    // Send reply
    char reply[] = "Hello from Server";
    write(client_fd, reply, strlen(reply));

    close(client_fd);
    close(server_fd);

    return 0;
}