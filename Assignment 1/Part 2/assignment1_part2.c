#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

int main(int argc, char **argv){
    int X, Y, fd[2], status;
    pid_t pid;

    if (pipe(fd) < 0){
        perror("pipe error");
        exit(0);
    }

    pid = fork();
    if (pid < 0) {
        perror("fork error");
        exit(0);
    }

    if(pid > 0) //parent
    {
        printf("\n A pipe is created for communication between parent (PID %d) and child\n", getpid());
        printf("\n Parent (PID %d) created a child (PID %d)\n", getpid(), pid);
        X = atoi(argv[1]);
        printf("\n Parent (PID %d) Reading X = %d from the user\n",getpid(), X);
        wait(NULL);
        read (fd[0],&Y, sizeof(Y));
        printf("\n Parent (PID %d) Reading Y from the pipe (Y = %d)\n",getpid(), Y);
        printf("\n Parent (PID %d) Adding X + Y = %d\n \n",getpid(),X+Y);


    }
    else //child

    {
        Y = atoi(argv[2]);
        printf("\n Child (PID %d) Reading Y = % d from the user\n", getpid(), Y);
        write(fd[1],&Y,sizeof(Y));
        printf("\n Child (PID %d) Writing Y into the pipe\n", getpid());
    }

    return 0;

}