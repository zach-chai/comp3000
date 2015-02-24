#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

int main (int argc, char *argv[])
{
    if(argv[1] == NULL || argc > 2)
        return 1;
    int status;
    char *execargs[4] = {"/bin/cat", "-bvt", argv[1], NULL};
    pid_t pid;
    pid = fork();
    if(pid==0)
    {
        printf("Child process\n");
        printf("Process id: %d \n", getpid());
        printf("Parent id: %d \n", getppid());
        printf("Process Group id: %d \n", getpgrp());
        execv("/bin/cat", execargs);
        return 1;
    }
    else
    {
        waitpid(pid, &status, 0);
        printf("Parent process\n");
        printf("Process id: %d \n", getpid());
        printf("Parent id: %d \n", getppid());
        printf("Process Group id: %d \n", getpgrp());
    }
    return 0;
}

