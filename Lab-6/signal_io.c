#include <stdio.h>
#include <unistd.h>
#include <signal.h>

void handler(int sig)
{
    char buffer[100];

    read(STDIN_FILENO, buffer, sizeof(buffer));

    printf("\nSignal Received!\n");
    printf("Input: %s\n", buffer);
}

int main()
{
    signal(SIGINT, handler);

    printf("Press Ctrl+C after typing something.\n");

    while(1)
    {
        pause();
    }

    return 0;
}