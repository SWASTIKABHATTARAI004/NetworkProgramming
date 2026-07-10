#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

int main()
{
    char buffer[100];

    int flags = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, flags | O_NONBLOCK);

    printf("Type something within 5 seconds...\n");

    sleep(5);

    if(read(STDIN_FILENO, buffer, sizeof(buffer)) > 0)
        printf("Input: %s\n", buffer);
    else
        printf("No input available.\n");

    return 0;
}