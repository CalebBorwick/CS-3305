#include <stdio.h>
#include <string.h>
#include<pthread.h>
#include "account.h"
#include <stdlib.h>
#include <stdbool.h>

int accountNum=0;
int depositNum = 0;
int clientNum = 0;
account* accountArray;
int ov=0;
int ovNum =0;


pthread_mutex_t lock;

void *thread_deposit(void *input){
    pthread_mutex_lock(&lock);  // ENTRY REGION

    char* tok;
    tok = strtok (input," ");
    tok = strtok (NULL," ");
    while(tok!=NULL){
        if (strcmp(tok,"d")==0){
            tok = strtok (NULL," a");
            int account = atoi(tok)-1;
            tok = strtok (NULL," ");
            int amount = atoi(tok);

            setBalance(&accountArray[account], getBalance(&accountArray[account]) + amount);

            setBalance(&accountArray[account],(getBalance(&accountArray[account])-(getDeposit(&accountArray[account]))));
            setTransaction(&accountArray[account],getTransaction(&accountArray[account])-1);
            if (getTransaction(&accountArray[account])<0){
                 setBalance(&accountArray[account],getBalance(&accountArray[account])-getTransactionFee(&accountArray[account]));
                }
        }
        tok = strtok (NULL," ");

    }
    pthread_mutex_unlock(&lock); // EXIT REGION

}

void *thread_client(void *input){
    pthread_mutex_lock(&lock);  // ENTRY REGION

    char* tok;
    tok = strtok (input," ");
    tok = strtok (NULL," ");

    while(tok!=NULL){
        if (strcmp(tok,"d")==0){//deposit
            //create temp var
            tok = strtok (NULL," a");
            int account = atoi(tok)-1;
            tok = strtok (NULL," ");
            int amount = atoi(tok);
            //set balance
            setBalance(&accountArray[account], getBalance(&accountArray[account]) + amount);
            //apply deposit fee
            setBalance(&accountArray[account],(getBalance(&accountArray[account])-(getDeposit(&accountArray[account]))));
            //apply transaction fee

            setTransaction(&accountArray[account],getTransaction(&accountArray[account])-1);
            if (getTransaction(&accountArray[account])<0){
                setBalance(&accountArray[account],getBalance(&accountArray[account])-getTransactionFee(&accountArray[account]));
            }

        }
        else if(strcmp(tok,"t")==0){// transfer
            //create temp vars
            tok = strtok (NULL," a");
            int account1 = atoi(tok)-1;
            tok = strtok (NULL," a");
            int account2 = atoi(tok)-1;
            tok = strtok (NULL," ");
            int amount = atoi(tok);
            //checks to see if there is enough money and overdraft
            if(amount>getBalance(&accountArray[account1])&&getOverdraft(&accountArray[account1])==0){
                printf("\n Request denied");
                break;
            }
            //set amounts into accounts
            setBalance(&accountArray[account1], getBalance(&accountArray[account1]) - amount);
            setBalance(&accountArray[account2], getBalance(&accountArray[account2]) + amount);

            // apply transfer fee
            setBalance(&accountArray[account1],(getBalance(&accountArray[account1])-(getTransfer(&accountArray[account1]))));
            setBalance(&accountArray[account2],(getBalance(&accountArray[account2])-(getTransfer(&accountArray[account2]))));

            //apply transaction fee if needed
            setTransaction(&accountArray[account1],getTransaction(&accountArray[account1])-1);
            if (getTransaction(&accountArray[account1])<0){
                setBalance(&accountArray[account1],getBalance(&accountArray[account1])-getTransactionFee(&accountArray[account1]));
            }
            setTransaction(&accountArray[account2],getTransaction(&accountArray[account2])-1);
            if (getTransaction(&accountArray[account2])<0){
                setBalance(&accountArray[account2],getBalance(&accountArray[account2])-getTransactionFee(&accountArray[account2]));
            }

            //apply overdraft
            if (getBalance(&accountArray[account1])< ovNum*-500){

            if (getBalance(&accountArray[account1])<0) {
                int overNum = 1;
                int temp = getBalance(&accountArray[account1]);
                while (temp < -500) {
                    overNum++;
                    temp = temp + 500;
                }
                int overdraftFee = overNum * getOverdraft(&accountArray[account1]);
                //applies overdraft limit
                if (overdraftFee > 5000) {
                    overdraftFee = 5000;
                }
                setBalance(&accountArray[account1], getBalance(&accountArray[account1]) - overdraftFee);
                ovNum = ovNum + overNum;
            }
            }
            }
        else if (strcmp(tok,"w")==0){//withdraw
            //create temp var
            tok = strtok (NULL," a");
            int account = atoi(tok)-1;
            tok = strtok (NULL," ");
            int amount = atoi(tok);

            //checks to see if there is enough money and overdraft
            if(amount>getBalance(&accountArray[account])&&getOverdraft(&accountArray[account])==0){
                printf("\n Request denied");
                break;
            }
            //set balance
            setBalance(&accountArray[account], getBalance(&accountArray[account]) - amount);
            //apply withdraw fee
            setBalance(&accountArray[account],(getBalance(&accountArray[account])-(getWithdraw(&accountArray[account]))));
            //apply transaction fee
            setTransaction(&accountArray[account],getTransaction(&accountArray[account])-1);
            if (getTransaction(&accountArray[account])<0){
                setBalance(&accountArray[account],getBalance(&accountArray[account])-getTransactionFee(&accountArray[account]));
            }
            //apply overdraft
            if (getBalance(&accountArray[account])< ovNum*-500){

                if (getBalance(&accountArray[account])<0) {
                    int overNum = 1;
                    int temp = getBalance(&accountArray[account]);
                    while (temp < -500) {
                        overNum++;
                        temp = temp + 500;
                    }
                    int overdraftFee = overNum * getOverdraft(&accountArray[account]);
                    //applies overdraft limit
                    if (overdraftFee > 5000) {
                        overdraftFee = 5000;
                    }
                    setBalance(&accountArray[account], getBalance(&accountArray[account]) - overdraftFee);
                    ovNum = ovNum + overNum;
                }
            }
        }
        tok = strtok (NULL," ");
    }

    pthread_mutex_unlock(&lock); // EXIT REGION
}


int main() {
    //Declare variables to read in the string
    FILE *ptr;
    int maxChar = 1000;
    char* file ="/Users/calebborwick/CLionProjects/CS_3305_Asn3/assignment_3_input_file.txt";
    ptr = fopen(file, "r");
    char str[maxChar];


    //checks if the file was able to be opened
    if (ptr == NULL){
        printf("Could not open file %s",file);
        return 1;
    }

    //gets size of arrays for clients, deposits and accounts.
    while (fgets(str, maxChar, ptr) != NULL) {
        if (str[0] == 'a') {
            accountNum++;
        }
        else if (str[0] == 'd') {
            depositNum++;

        } else if (str[0] == 'c') {
            clientNum++;
        }
    }
    accountArray=malloc(accountNum* sizeof(struct account*));
    pthread_t depThreads[depositNum];
    pthread_t clientThreads[clientNum];

    fclose(ptr);

    if (pthread_mutex_init(&lock, NULL) != 0)
    {
        printf("\n mutex init failed\n");
        return 1;
    }

    ptr = fopen(file, "r");
    int ia = 0;
    int id = 0;
    int ic = 0;
    int iter = 0;
    int dTreadErr, cTreadErr;
    char strArray[(accountNum+clientNum+depositNum)][maxChar];
    //loads the contents of the output file into str
    while (fgets(str, maxChar, ptr) != NULL) {
        char *tok;
        char line[strlen(str)];
        strcpy(strArray[iter], str);
        int check =0;
        tok = strtok(str, " ");
        if (str[0] == 'a') {
            tok = strtok(NULL, " ");//type
            tok = strtok(NULL, " ");//type value
            char type = tok[0];
            //printf("type = %s \n", tok);

            tok = strtok(NULL, " ");//d
            tok = strtok(NULL, " ");//deposit val
            //printf("dep = %s \n", tok);
            int dep = atoi(tok);

            tok = strtok(NULL, " ");//w
            tok = strtok(NULL, " ");//withdraw val
            //printf("with = %s \n", tok);
            int with = atoi(tok);

            tok = strtok(NULL, " ");//t
            tok = strtok(NULL, " ");// transfer value
            int transf = atoi(tok);
            //printf("transf = %d \n", transf);


            tok = strtok(NULL, " ");// transaction
            tok = strtok(NULL, " ");//transaction value
            int transa = atoi(tok);
            //printf("transa = %d \n", transa);


            tok = strtok(NULL, " ");//transaction fee
            int transfee = atoi(tok);
          // printf("transfee = %d \n", transfee);//

            tok = strtok(NULL, " ");//overdraft
            tok = strtok(NULL, " "); // overdraft bool
            int over=0;
            if (strcmp(tok, "N") == 0){
                over=0;
            }
            else if (strcmp(tok, "Y") == 0){
                tok = strtok(NULL, " ");//overdraft value
                over = atoi(tok);
            }
            //printf("over = %d \n\n", over);

            accountArray[ia]= *createAccount(type, dep, with, transf, transa, transfee, over, 0);
            ia++;

        } else if (str[0] == 'd') {
            dTreadErr =  pthread_create(&depThreads[id], NULL, &thread_deposit, (void*)strArray[iter]);
            if (dTreadErr != 0){
                printf("\n Error creating thread %d", id);
            }
            id++;

        } else if (str[0] == 'c') {

            if (check ==0) {
                check=1;
                for (id = 0; id < depositNum; id++) {
                    pthread_join(depThreads[id], NULL);
                }
            }

            cTreadErr = pthread_create(&clientThreads[ic], NULL, &thread_client, (void*)strArray[iter]);
            if (cTreadErr != 0){
                printf("\n Error creating thread %d", ic);
            }
            ic++;
        }
        iter++;
    }


    for (ic = 0; ic<clientNum; ic++){
        pthread_join(clientThreads[ic], NULL);
    }

    pthread_mutex_destroy(&lock);


    //writing to output file
    char* outFile ="/Users/calebborwick/CLionProjects/CS_3305_Asn3/assignment_3_output_file.txt ";
    FILE *output= fopen(outFile,"w");
    for (int i=0; i<accountNum;i++) {
        if (getType(&accountArray[i])=='b'){
            fprintf(output,"a%d type business %d\n", i+1, getBalance(&accountArray[i]));
        }
        else{
            fprintf(output,"a%d type personal %d\n", i+1, getBalance(&accountArray[i]));
        }

    }
    return 0;
}
