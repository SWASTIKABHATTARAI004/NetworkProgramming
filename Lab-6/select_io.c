#include <stdio.h>
#include <unistd.h>
#include <sys/select.h>

int main()
{
    fd_set readfds;
    struct timeval tv;

    FD_ZERO(&readfds);
    FD_SET(STDIN_FILENO, &readfds);

    tv.tv_sec = 10;
    tv.tv_usec = 0;

    printf("Type within 10 seconds...\n");

    int ret = select(STDIN_FILENO + 1, &readfds, NULL, NULL, &tv);

    if(ret > 0)
    {
        char buffer[100];
        read(STDIN_FILENO, buffer, sizeof(buffer));
        printf("Input: %s\n", buffer);
    }
    else
    {
        printf("Timeout.\n");
    }

    return 0;
}