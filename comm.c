#include "hospital.h"
#include "comm.h"

/****
FUNZIONI RELATIVE AI SEMAFORI
*****/


//genera "num" semafori aventi la stessa key passata come parametro
int createSem(int key, int num){
    int semid;
    if ((semid = semget((key_t) key, num, 0600 | IPC_CREAT | IPC_EXCL)) == -1){ 
        if (errno == EEXIST){ // se il semaforo è già stato creato
            semid = semget((key_t) key, num, 0600); //si limita a prendere l' id di quel semaforo
        }
    }
    return semid;
}

//distrugge il semaforo 
void destroySem(int semid){ 
    if (semctl(semid, 0, IPC_RMID, 0) == -1){
            printf("Error destroySem (%d)\n", errno);
            exit(EXIT_FAILURE);
    }
}

//inizializza il semaforo indicato dai parametri al valore passato come terzo parametro
void initSem(int semid, int semnum, int val){
        union semun arg;
        arg.val = val;
        if (semctl(semid, semnum, SETVAL, arg) == -1){
                printf("Error initSem (%d)\n", errno);
                exit(EXIT_FAILURE);
        }
}

//decrementa il semaforo
void semReserve(int semid, int semnum){
        struct sembuf sops;
        sops.sem_num = semnum;
        sops.sem_op = -1;
        sops.sem_flg = 0;
        if (semop(semid, &sops, 1) == -1) {
            if (errno != EINTR) {
                printf("Error semReserve (%d)\n", errno);
                exit(EXIT_FAILURE);
            }
        }
}

//incrementa il semaforo
void semRelease(int semid, int semnum){
        struct sembuf sops;
        sops.sem_num = semnum;
        sops.sem_op = +1;
        sops.sem_flg = 0;
        if (semop(semid, &sops, 1) == -1) {
            if (errno != EINTR) {
                printf("Error semRelease (%d)\n", errno);
                exit(EXIT_FAILURE);
            }
        }
}

//ottiene il valore del semaforo
int semGetVal(int semid, int semnum){
    int res;
    if((res = semctl(semid, semnum, GETVAL, 0)) < 0){
        printf("Error semGetVal (%d)\n", errno);
        exit(EXIT_FAILURE);
    }
    return res;
}

/****
FUNZIONI RELATIVE ALLE CODE DI MESSAGGI
*****/

// Crea una coda di messaggi con la key passata come parametro
int createMsgQ(int key, bool attachIfExists) {
    int msgqid;
    if ( (msgqid = msgget(key, 0600 | IPC_CREAT | IPC_EXCL)) < 0 ) {
        if (errno == EEXIST && attachIfExists){ 
            msgqid = msgget(key, 0600);
        } else {
            printf("Error msgget IPC_CREAT (%d)\n", errno);
            exit(EXIT_FAILURE);
        }
    }
    return msgqid;
}


//distrugge la coda di messaggi
void destroyMsgQ(int msgqid){
    if (msgctl(msgqid, IPC_RMID, NULL) == -1) { 
        printf("Error msgctl IPC_RMID (%d)\n", errno);
        exit(EXIT_FAILURE);
    }
}

// Scrivo messaggi in coda
void sendMessage(int msgid, void *msg, int msgSize) {
    if (msgsnd(msgid, msg, msgSize - sizeof(long), 0) == -1){ 
        if (errno != EINTR) {
            printf("Error msgsnd (%d)\n", errno);    
            exit(EXIT_FAILURE);
        }
    }
}
    
// Ottengo messaggi in base al tipo
bool recvMessage(int msgid, void *msg, int msgSize, long msgtype) {
    if ( msgrcv(msgid, msg, msgSize - sizeof(long), msgtype, IPC_NOWAIT) < 0){
        if (errno == ENOMSG || errno == EINTR){
            return false;
        } else {
            printf("Error msgrecv (%d)\n", errno);    
            exit(EXIT_FAILURE);
        }
    }
    return true;
}

/***
MISC
***/


// aspetta che muoiano tutti i figli di un processo
void waitAllChild(){
    while (waitpid(-1, NULL, 0)) {
       if (errno == ECHILD) {
          break;
       }
    }
}


// genera un numero random compreso tra min_num e max_num (compresi)
int getRand(int minNum, int maxNum){
    int lowNum = 0, hiNum = 0;
    if (minNum<maxNum) {
        lowNum = minNum;
        hiNum = maxNum + 1; // +1 per includere il max 
    } else {
        lowNum = maxNum + 1; // +1 per includere il max
        hiNum = minNum;
    }
    return (rand()%(hiNum-lowNum))+lowNum;
}

