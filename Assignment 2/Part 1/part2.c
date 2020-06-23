#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

//counts the number of processes for the size of array
int counter(char * str){
    int i = 0;
    int pCounter=0;
    while(str[i]!=NULL){
        if(str[i]=='p'){
            pCounter++;
        }
        i++;
    }
    return(pCounter);
}

int main() {
    //Declare variables to read in the string
    int maxChar = 1000;
    char str[maxChar];
    FILE *ptr;
    ptr =fopen("/Users/calebborwick/CLionProjects/CS_3305_Asn2_Part2/cpu_scheduling_input_file.txt ", "r");
    FILE *output;
    output= fopen("/Users/calebborwick/CLionProjects/CS_3305_Asn2_Part2/cpu_scheduling_input_file.txt ","w");

    //checks if the file was able to be opened
    if (!ptr){
        printf("Could not open file %s",ptr);
        return 1;
    }

    int curQueue;
    int timeQ;

    //loads the contents of the output file into str
    while (fgets(str, maxChar, ptr) != NULL) {
        //printf("%s\n", str);
        int pCounter;
        pCounter=counter(str);
        char *proccess[pCounter];
        int waitTime[pCounter];
        //splits the str into tokens
        char * tok;
        tok = strtok (str," ");
        int i=0;
        //Loads all the tokens in the queue
        while (tok != NULL)
        {
            // checks to see what the tok is then reads next char in order to store it
            if (strcmp(tok,"q")==0){
                tok = strtok (NULL, " ");
                curQueue=atoi(tok);
                proccess[pCounter];
                waitTime[pCounter];
                tok = strtok (NULL, " ");

            }
            else if (strcmp(tok,"tq")==0){
                tok = strtok (NULL, " ");
                timeQ=atoi(tok);
                tok = strtok (NULL, " ");

            }
            else {
                proccess[i]=tok;
                tok = strtok (NULL, " ");
                int wait=atoi(tok);
                waitTime[i]=wait;
                i=i+1;
                tok = strtok (NULL, " ");

            }
        }

        //external calls to the queueing process
        //FCFS
        fprintf(output,"Ready Queue %d Applying FCFS Scheduling:\n\n", curQueue);
        fprintf(output,"Order of selection by CPU: :\n");
        int j=0;
        while (j< i){
            fprintf(output,"%s ",proccess[j]);
            j=j+1;
        }
        fprintf(output,"\n\nIndividual waiting times for each process: \n");
        int avgTime=0;
        float totalTime=0;
        j=0;
        while (j< i){
            fprintf(output,"%s=%d\n",proccess[j],avgTime);
            totalTime=avgTime+totalTime;
            avgTime=avgTime+waitTime[j];
            j=j+1;
        }
        totalTime=totalTime/i;
        fprintf(output,"\nAverage waiting time = %0.1f\n\n",totalTime);


        //SJF
        fprintf(output,"Ready Queue %d Applying SJF Scheduling:\n\n", curQueue);
        char *SJFproccess[pCounter];
        int SJFwaitTime[pCounter];
        j=0;
        while (j<i){
            SJFproccess[j]=proccess[j];
            SJFwaitTime[j]=waitTime[j];
            j++;
        }
        int m,n, tempWait;
        char *tempProc;
        for(m=0;m<i;m++){
            for(n=m+1;n<i;n++){
                if(SJFwaitTime[m]>SJFwaitTime[n]){
                    tempWait=SJFwaitTime[m];
                    tempProc=SJFproccess[m];

                    SJFwaitTime[m]=SJFwaitTime[n];
                    SJFproccess[m]=SJFproccess[n];

                    SJFwaitTime[n]=tempWait;
                    SJFproccess[n]=tempProc;
                }
            }
        }
        fprintf(output,"Order of selection by CPU: :\n");
        j=0;
        while (j< i){
            fprintf(output,"%s ",SJFproccess[j]);
            j++;
        }
        fprintf(output,"\n\nIndividual waiting times for each process: \n");
        avgTime=0;
        totalTime=0;
        j=0;
        while (j< i){
            fprintf(output,"%s=%d\n",SJFproccess[j],avgTime);
            totalTime=avgTime+totalTime;
            avgTime=avgTime+SJFwaitTime[j];
            j=j+1;
        }
        totalTime=totalTime/i;
        fprintf(output,"\nAverage waiting time = %0.1f\n\n",totalTime);


        //Round Robin
        fprintf(output,"Ready Queue %d Applying Round Robin Scheduling:\n\n", curQueue);
        fprintf(output,"Order of selection by CPU: \n");
        //Helper arrays
        char *RRproccess[pCounter*3];
        int RRwaitTime[pCounter*3];
        char *Endproccess[pCounter*3];
        int EndwaitTime[pCounter*3];
        int BegTime[pCounter*3];
        //Loop to import the original array values into the new helper arrays
        j=0;
        while (j<i){
            RRproccess[j]=proccess[j];
            RRwaitTime[j]=waitTime[j];
            j++;
        }

        //declare variables used in round robin
        int arrayCount=i;
        int pLen=i;
        int tq=timeQ;
        int endCount=0;
        int arrivalT=0;

        //for loop to iterate through the array adding onto end with next wait times
        for (j=0; j<arrayCount;){
            if (tq==RRwaitTime[j]){
                Endproccess[endCount]=RRproccess[j];
                arrivalT=arrivalT+tq;
                if (j<=pLen){
                    EndwaitTime[endCount]=tq;
                }
                else {
                    for(int k=0; k<pLen;k++){
                        if (Endproccess[endCount]==RRproccess[k]){
                            if (k==0){
                                EndwaitTime[endCount]=arrivalT;
                            }
                            else {
                                EndwaitTime[endCount] = arrivalT - BegTime[k - 1];
                            }

                        }
                    }
                }
                endCount++;
            }
            else if(tq>RRwaitTime[j]){
                Endproccess[endCount]=RRproccess[j];
                arrivalT=arrivalT+RRwaitTime[j];
                if (j<=pLen){
                    EndwaitTime[endCount]=RRwaitTime[j];
                }
                else {
                    //loop to find the position the proccess was started in in order to subtract the total from the begining
                    for(int k=0; k<pLen;k++){
                        if (Endproccess[endCount]==RRproccess[k]){
                            if (k==0){
                                EndwaitTime[endCount]=arrivalT;
                            }
                            else {
                                EndwaitTime[endCount] = arrivalT - BegTime[k - 1];
                            }

                        }
                    }
                }
                endCount++;
            }
            else if(tq<RRwaitTime[j]){
                RRproccess[arrayCount]=RRproccess[j];
                int tempTime = (RRwaitTime[j]-4);
                RRwaitTime[arrayCount]=tempTime;
                arrivalT=arrivalT+tq;
                arrayCount++;

            }
            BegTime[j]=arrivalT;
            j++;

        }
        j=0;
        while (j< arrayCount){
            fprintf(output,"%s ",RRproccess[j]);
            j++;
        }

        fprintf(output,"\n\nTurnaround times for each process: ");
        j=0;
        while(j<15){
            fprintf(output, "\n%s=%d", Endproccess[j],EndwaitTime[j]);
            j=j+1;

        }
        fprintf(output,"\n\n");
    }
    fclose(ptr);
    return 0;
}




