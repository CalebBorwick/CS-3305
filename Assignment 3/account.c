//
// Created by CalebBorwick on 2019-11-25.
//
#include "account.h"
#include <stdlib.h>
#include <stdio.h>

account *createAccount(char typ, int dep, int with, int transf, int transa, int transFee, int over, int bal){
    account *new = (account *) malloc(sizeof(account));
    new->type = typ;
    new->deposit = dep;
    new->withdraw=with;
    new->transfer = transf;
    new->transaction = transa;
    new->transactionFee = transFee;
    new->overdraft = over;
    new->balance=bal;
    return new;
}

char getType(account *a1){
    return a1->type;
}
void setType(account *a1, char typ){
    a1->type=typ;
}
int getDeposit(account *a1){
    return a1->deposit;
}
void setDeposit(account *a1, int dep){
    a1->deposit = dep;
}
int getWithdraw(account *a1) {
    return a1->withdraw;
}
void setWithdraw(account *a1, int with){
    a1->withdraw=with;
}
int getTransfer(account *a1){
    return a1->transfer;
}
void setTransfer(account *a1, int tranf){
    a1->transfer=tranf;
}
int getTransaction(account *a1){
    return a1->transaction;
}
void setTransaction(account *a1,int transa){
    a1->transaction=transa;
}
int getTransactionFee(account *a1){
    return a1->transactionFee;
}
void setTransactionFee(account *a1,int fee){
    a1->transactionFee=fee;
}
int getOverdraft(account *a1){
    return a1->overdraft;
}
void setOverdraft(account *a1,int over){
    a1->overdraft=over;
}
int getBalance(account *a1){
    return a1->balance;
}
void setBalance(account *a1,int bal){
    a1->balance=bal;
}
