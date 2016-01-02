typedef enum {false, true} bool; 

union semun {
        int val;
        struct semid_ds* buf;
        unsigned short* array;
        #if defined(__linux__)
        struct seminfo* __buf;
        #endif
};

int createSem(int key, int num){ //genera "num" semafori aventi la stessa key passata come parametro
        int semid;
        if ((semid = semget((key_t) key, num, 0600 | IPC_CREAT | IPC_EXCL)) == -1){
                if (errno == EEXIST){//se il semaforo è già stato creato
                        semid = semget((key_t) key, num, 0600);//si limita a prendere l' id di quel semaforo
                }
        }
        return semid;
}

int destroySem(int semid){ //distrugge il semaforo 
        if (semctl(semid, 0, IPC_RMID, 0) == -1){
                printf("Error semctl\n");
                exit(EXIT_FAILURE);
        }
}

void initSem(int semid, int semnum, int val){//inizializza il semaforo indicato dai parametri al valore passato come terzo parametro
        union semun arg;
        arg.val = val;
        if (semctl(semid, semnum, SETVAL, arg) == -1){
                printf("Error semctl\n");
                exit(EXIT_FAILURE);
        }
}

void semReserve(int semid, int semnum){//decrementa il semaforo
        struct sembuf sops;
        sops.sem_num = semnum;
        sops.sem_op = -1;
        sops.sem_flg = 0;
        if (semop(semid, &sops, 1) == -1) {
                printf("Error semop\n");
                exit(EXIT_FAILURE);
        }
}


void semRelease(int semid, int semnum){//incrementa il semaforo
        struct sembuf sops;
        sops.sem_num = semnum;
        sops.sem_op = +1;
        sops.sem_flg = 0;
        if (semop(semid, &sops, 1) == -1) {
                printf("Error semop\n");
                exit(EXIT_FAILURE);
        }
}

void loadConfig(int* numPazienti, int* numReparti, int* maxTime){
        FILE* confFile = fopen("hospital.conf", "r");
        if (confFile) {
            //read conf
            fclose(confFile);
        }
}


