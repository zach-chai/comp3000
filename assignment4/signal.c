#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>

#define SIZE 32
#define SLEEP_TIME 500000000

void handleChild(int sig)
{
    char buf[SIZE];

    memset(buf, '\0', (int)SIZE);
    sprintf(buf, "Signal Number %d\n", sig);
    write(STDOUT_FILENO, buf, SIZE);

    memset(buf, '\0', (int)SIZE);
    sprintf(buf, "Child received signal\n");
    write(STDOUT_FILENO, buf, SIZE);

    _exit(0);
}

void handleParent(int sig)
{
    char buf[SIZE];

    memset(buf, '\0', (int)SIZE);
    sprintf(buf, "Signal Number %d\n", sig);
    write(STDOUT_FILENO, buf, SIZE);

    memset(buf, '\0', (int)SIZE);
    sprintf(buf, "Parent received signal\n");
    write(STDOUT_FILENO, buf, SIZE);

    _exit(0);
}

int main ()
{
    int status;
    pid_t child_pid, parent_pid;

    parent_pid = getpid();
    child_pid = fork();
    if(child_pid==0)
    {
        signal(SIGUSR1, handleChild);
        while(1)
        {
            printf("Child waiting\n");
            nanosleep((struct timespec[]){{0, SLEEP_TIME}}, NULL);
        }
    }
    else
    {
        signal(SIGCHLD, handleParent);
        sleep(1);
        kill(child_pid, SIGUSR1);
        while(1)
        {
            printf("Parent waiting\n");
            nanosleep((struct timespec[]){{0, SLEEP_TIME}}, NULL);
        }
    }
    return 0;
}