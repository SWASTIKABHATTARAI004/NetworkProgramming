#include <stdio.h>
#include <unistd.h>

int main()
{
    char buffer[100];

    printf("Enter a message: ");

    read(STDIN_FILENO, buffer, sizeof(buffer));

    printf("You entered: %s\n", buffer);

    return 0;
}
