//
// Created by CalebBorwick on 2019-11-25.
//
#ifndef CS_3305_ASN3_ACCOUNT_H
#define CS_3305_ASN3_ACCOUNT_H
typedef struct account{
    char type;
    int deposit;
    int withdraw;
    int transfer;
    int transaction;
    int transactionFee;
    int overdraft;
    int balance;
} account;
account *createAccount(char typ, int dep, int with, int transf, int transa, int transFee, int over, int bal);

char getType(account* a1);
void setType(account* a1,char typ);

int getDeposit(account* a1);
void setDeposit(account* a1,int dep);

int getWithdraw(account* a1);
void setWithdraw(account* a1,int with);

int getTransfer(account* a1);
void setTransfer(account* a1,int tranf);

int getTransaction(account* a1);
void setTransaction(account* a1,int transa);

int getTransactionFee(account* a1);
void setTransactionFee(account* a1,int fee);

int getOverdraft(account* a1);
void setOverdraft(account* a1,int over);

int getBalance(account* a1);
void setBalance(account* a1,int bal);

#endif //CS_3305_ASN3_ACCOUNT_H
