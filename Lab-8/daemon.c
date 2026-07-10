#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <time.h>

int main()
{
    pid_t pid;

    // Create child process
    pid = fork();

    if (pid < 0)
    {
        printf("Fork failed!\n");
        exit(EXIT_FAILURE);
    }

    // Parent process exits
    if (pid > 0)
    {
        printf("Daemon started with PID: %d\n", pid);
        exit(EXIT_SUCCESS);
    }

    // Create new session
    if (setsid() < 0)
    {
        exit(EXIT_FAILURE);
    }

    // Set file permissions
    umask(0);

    // Change working directory
    chdir("/");

    // Close standard input, output, and error
    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);

    while (1)
    {
        FILE *fp;
        time_t now;

        fp = fopen("/tmp/daemon_log.txt", "a");

        if (fp != NULL)
        {
            now = time(NULL);
            fprintf(fp, "Daemon is running at %s", ctime(&now));
            fclose(fp);
        }

        sleep(5);
    }

    return 0;
}