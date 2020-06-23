#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <errno.h>

int main ()
{
    pid_t i, j, child_1, child_2, child_11;
    int status;

    child_1=fork();

    if (child_1 <0) // fork unsuccessful
    {
        printf("fork unsuccessful");
        exit(1);
    }

    if (child_1>0) //parent
    {
        i = getpid();
        printf("\n parent process (PID %d) created child_1 (PID %d)\n", i, child_1);
        printf("\n parent (PID %d) is waiting for child_1 (PID %d) to complete before creating child_2\n",i, child_1);
        wait(NULL);
        child_2=fork();
        if (child_2 <0) // fork unsuccessful
        {
            printf("fork unsuccessful");
            exit(1);
        }
        if (child_2 == 0) //child 2
        {
            i= getpid();
            printf("\n child_2 (PID %d) is calling an external program external_program.out and leaving child_2\n", i);
            status = execl("./external_program.out", NULL);
            if (status < 0)
            {
                printf("\n main: errno is %d\n",errno);
            }

        }
        if (child_2 > 0){
            i = getpid();
            printf("\n parent (PID %d) created child_2 (PID %d)\n", i, child_2);
            wait(NULL);
        }
    }

    if (child_1==0) //child 1
    {
        child_11 = fork();
        wait(NULL);
        if (child_11 <0) // fork unsuccessful
        {
            printf("fork unsuccessful");
            exit(1);
        }
        if (child_11 == 0) //child 11
        {
            i = getpid();
            j = getppid();
            printf("\n child_1 (PID %d) created child_1.1 (PID %d) \n", j, i);
            printf("\n child_1 (PID %d) is now completed \n", j);
            exit(1);
        }
    }


}