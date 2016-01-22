#include "hospital.h"
#include "comm.h"

/****
FUNZIONI RELATIVE AI SEMAFORI
*****/


//genera "num" semafori aventi la stessa key passata come parametro
int createSem(int key, int num){
    int semid;
    if ((semid = semget((key_t) key, num, 0600 | IPC_CREAT | IPC_EXCL)) == -1){ 
        if (errno == EEXIST){ // se il semaforo � gi� stato creato
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

int semGetVal(int semid, int semnum){
    int res;
    if((res = semctl(semid, semnum, GETVAL, 0)) < 0){
        printf("Error semGetVal (%d)\n", errno);

        switch (errno){
            /*case EACCESS:
                printf("EACCESS (permission denied)\n"); 
                break;*/
            case EFAULT:
                printf("EFAULT (invalid address pointed to by arg argument)\n");
                break;
            case EIDRM:
                printf("EIDRM (semaphore set was removed)\n");
                break;
            case EINVAL:
                printf("EINVAL (set doesn't exist, or semid is invalid)\n");
                break;
            case EPERM:
                printf("EPERM (EUID has no privileges for cmd in arg)\n");
                break;
            case ERANGE:
                printf("ERANGE (semaphore value out of range)\n");
                break;                 
        }

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
    if (msgsnd(msgid, msg, msgSize - sizeof(long), 0) == -1){ // METTERLA NO WAIT E TORNARE TRUE SE MANDATO, FALSE SE NON MANDATO (CODA PIENA)
        if (errno != EINTR) {
            printf("Error msgsnd (%d)\n", errno);    
            exit(EXIT_FAILURE);
        }
    }
}
    
// Ottengo messaggi in base al tipo
bool recvMessage(int msgid, void *msg, int msgSize, long msgtype) {
    int dim = msgrcv(msgid, msg, msgSize - sizeof(long), msgtype, IPC_NOWAIT);
    //printf("\n rcv: %d\n", dim);
    if ( dim < 0){
        if (errno == ENOMSG || errno == EINTR){
            return false;
        } else {
            printf("Error msgrecv (%d)\n", errno);    
            exit(EXIT_FAILURE);
        }
    }
    return true;
}

/*
void getInfo(int qid, struct msqid_ds *queue_ds){
    printf("[info] ID CODA: %d\n", qid);
    if (msgctl(qid, IPC_STAT, queue_ds ) == -1 ) { 
        printf("msgctl error (%d)\n", errno);
        exit(EXIT_FAILURE);
    }
    printf("Num of messages in queue: %d\n", (int) (*queue_ds).msg_qnum);
}
*/

void waitAllChild(){
    while (waitpid(-1, NULL, 0)) {
       if (errno == ECHILD) {
          break;
       }
    }
}


// genera un numero casuale entro un range
int getRand(int min, int max){
    int casuale;
    do{
        casuale = rand()%10;
    }while(casuale < min || casuale > max); 
    return casuale;

}

