#include <stdlib.h>
#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>
#include <pthread.h>

//Declare/initialize global variables
int z =0, x = 10,y= 20;

//thread
void *thread_sum_x_y(void *num){
    z = (x+y);
    return(0);
};

int main() {
    //initialize variables
    pid_t child_1;
    pthread_t pthread_1;

    child_1 = fork(); //fork and create child process

    if(child_1<0){ // unsuccessful fork
        printf("fork unsuccessful");
        exit(1);
    }

    if (child_1 >0){ //Parent
        wait(NULL);
        printf("parent process (PID %d) \n",getpid());
        printf("Printing z after child process z=%d \n",z);
        pthread_create(&pthread_1, NULL, thread_sum_x_y, NULL);
        pthread_join(pthread_1, NULL);
        printf("Printing z after thread process z=%d\n",z);
    }

    if (child_1 ==0){ //child
        printf("child process(PID %d)and parent process (PID %d) \n",getpid(),getppid());
        z = x+y;
    }
    return 0;
}