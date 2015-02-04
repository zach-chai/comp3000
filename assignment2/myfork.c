#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

int main (int argc, char *argv[])
{
    if(argv[1] == NULL)
        return 1;
    int status;
    pid_t pid;
    pid = fork();
    if(pid==0)
    {
        printf("Child process\n");
        printf("Process id: %d \n", getpid());
        printf("Parent id: %d \n", getppid());
        printf("Process Group id: %d \n", getpgrp());
        execl("/bin/cat", "/bin/cat", "-bvt", argv[1], NULL);
        perror("Child failed to exec ls");
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

