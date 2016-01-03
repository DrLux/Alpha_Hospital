typedef enum {false, true} bool; 

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


void triage();
//void getMessage(int msgid, struct ddqueue *msg, long msgtype);
//void setMessage(int msgid, struct ddqueue *msg, long msgtype);
int createCodeMessage (int key);


//struttura che definisce il paziente nella coda nel triage
struct paziente {
 char *malattia; /* Nome della malattia */
 int gravita; /* Indice (da 1 a 10) di gravita della malattia del paziente */
 int reparto; /* Reparto associato alla specifica malattia */
}; 

// Crea una coda di messaggi con la key passata come parametro
int createCodeMessage (int key){
	int msgid; 
	if ((msgid = msgget( key, 0600 | IPC_CREAT | IPC_EXCL)) < 0)
		printf("Error msgget\n");
                exit(EXIT_FAILURE);
	return msgid;
}

/*// Scrivo messaggi in coda
void setMessage(int msgid, struct ddqueue *msg, long msgtype){
	if( msgsnd(msgid, &msg, sizeof(msg), IPC_NOWAIT) == -1) 
		printf("\nmsgsnd error");	 
		exit(EXIT_FAILURE);
}*/	
	
/*// Ottengo messaggi in base al tipo -------------> DA FINIRE
void getMessage(int msgid, struct ddqueue *msg, long msgtype) {
        if (msgrcv(msgid, msg, sizeof(*msg), msgtype, IPC_NOWAIT) == -1) { 
                printf("None message with type %ld\n", msgtype);
				exit(EXIT_FAILURE);
        } else {
                //(*msg).mtext[63] = '\0';
                //printf("Contenuto: \"%s\"\n", (*msg).mtext);
                //fflush(stdout);
        }
}*/



int createSem(int key, int num){ //genera "num" semafori aventi la stessa key passata come parametro
    int semid;
    if ((semid = semget((key_t) key, num, 0600 | IPC_CREAT | IPC_EXCL)) == -1){ //con questi permessi il semfaro è invisibile ad altri utenti
        if (errno == EEXIST){ // se il semaforo è già stato creato
            semid = semget((key_t) key, num, 0600);//si limita a prendere l' id di quel semaforo
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



