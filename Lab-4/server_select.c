#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/select.h>

#define PORT 8080

int main()
{
    int server_fd, client_fd;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_len;
    fd_set readfds;
    char buffer[1024];

    server_fd = socket(AF_INET, SOCK_STREAM, 0);

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr));

    listen(server_fd, 5);

    printf("Server waiting for connections...\n");

    while (1)
    {
        FD_ZERO(&readfds);

        FD_SET(server_fd, &readfds);

        select(server_fd + 1, &readfds, NULL, NULL, NULL);

        if (FD_ISSET(server_fd, &readfds))
        {
            addr_len = sizeof(client_addr);

            client_fd = accept(server_fd,
                               (struct sockaddr *)&client_addr,
                               &addr_len);

            printf("Client Connected\n");

            memset(buffer, 0, sizeof(buffer));

            read(client_fd, buffer, sizeof(buffer));

            printf("Client: %s\n", buffer);

            write(client_fd,
                  "Hello from Server using select()",
                  32);

            close(client_fd);
        }
    }

    close(server_fd);

    return 0;
}