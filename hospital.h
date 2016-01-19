typedef enum {false, true} bool; 


//struttura che definisce il paziente
struct paziente {
    char *malattia; /* Nome della malattia */
    //int mtype; /* GRAVITA Indice (da 1 a 10) di gravita della malattia del paziente */
    //int reparto; /* Reparto associato alla specifica malattia */
};

// non compila su mac
#if defined(__linux__)
union semun {
        int val;
        struct semid_ds* buf;
        unsigned short* array;
        #if defined(__linux__)
        struct seminfo* __buf;
        #endif
};
#endif

int createSem(int key, int num);
void destroySem(int semid);
void initSem(int semid, int semnum, int val);
void semReserve(int semid, int semnum);
void semRelease(int semid, int semnum);

int createMsgQ(int key);
void getMessage(int msgid, struct paziente *msg, long msgtype);
void setMessage(int msgid, struct paziente *msg, long msgtype);


int createSem(int key, int num){ //genera "num" semafori aventi la stessa key passata come parametro
    int semid;
    if ((semid = semget((key_t) key, num, 0600 | IPC_CREAT | IPC_EXCL)) == -1){ 
        if (errno == EEXIST){ // se il semaforo è già stato creato
            semid = semget((key_t) key, num, 0600); //si limita a prendere l' id di quel semaforo
        }
    }
    return semid;
}

void destroySem(int semid){ //distrugge il semaforo 
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

//********

// Crea una coda di messaggi con la key passata come parametro
int createMsgQ(int key) {
    int msgqid; 
    if ( (msgqid = msgget( key, 0600 | IPC_CREAT | IPC_EXCL)) < 0 ) {
        printf("Error msgget IPC_CREAT\n");
        exit(EXIT_FAILURE);
    }
    return msgqid;
}

void destroyMsgQ(int msgqid){
    if (msgctl(msgqid, IPC_RMID, NULL) == -1) { //elimina la coda di messaggi CORTOCIRCUITO
        printf("Error msgctl IPC_RMID\n");
        exit(EXIT_FAILURE);
    }
}

// Scrivo messaggi in coda
void sendMessage(int msgqid, struct paziente *msg, long msgtype) {
    if (msgsnd(msgqid, &msg, sizeof(msg), msgtype) == -1){   
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



