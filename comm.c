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
            printf("Error semctl\n");
            exit(EXIT_FAILURE);
    }
}

//inizializza il semaforo indicato dai parametri al valore passato come terzo parametro
void initSem(int semid, int semnum, int val){
        union semun arg;
        arg.val = val;
        if (semctl(semid, semnum, SETVAL, arg) == -1){
                printf("Error semctl\n");
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
                printf("Error semop\n");
                exit(EXIT_FAILURE);
        }
}

//incrementa il semaforo
void semRelease(int semid, int semnum){
        struct sembuf sops;
        sops.sem_num = semnum;
        sops.sem_op = +1;
        sops.sem_flg = 0;
        if (semop(semid, &sops, 1) == -1) {
                printf("Error semop\n");
                exit(EXIT_FAILURE);
        }
}

/****
FUNZIONI RELATIVE ALLE CODE DI MESSAGGI
*****/

// Crea una coda di messaggi con la key passata come parametro
int createMsgQ(int key, bool attachIfExists) {
    int msgqid;
    if ( (msgqid = msgget(key, 0600 | IPC_CREAT | IPC_EXCL)) < 0 ) {
        printf("%d\n", errno);
        if (errno == EEXIST && attachIfExists){ 
            msgqid = msgget(key, 0600);
        } else {
            printf("Error msgget IPC_CREAT\n");
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
void sendMessage(int msgid, char* msgp, size_t length) {
    if (msgsnd(msgid, msgp, length, 0) == -1){   
        printf("Error msgsnd\n");    
        exit(EXIT_FAILURE);
    } 
}
    
// Ottengo messaggi in base al tipo
void recvMessage(int msgid, struct paziente *msg, long msgtype) {
    if (msgrcv(msgid, msg, sizeof(*msg), msgtype, IPC_NOWAIT) == -1) { 
            printf("None message with type %ld\n", msgtype);
            exit(EXIT_FAILURE);
    } else {
            printf("Contenuto: %s \n", (*msg).malattia);
            fflush(stdout);
    }
}

    
//genera un numero randomico entro un certo range di valori
int getRand(int min , int max){
        int casuale = 0;
        while ( casuale < min || casuale > max)
                casuale = rand()%10;
        return casuale;
}

