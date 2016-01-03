typedef enum {false, true} bool; 

/*
union semun {
        int val;
        struct semid_ds* buf;
        unsigned short* array;
        #if defined(__linux__)
        struct seminfo* __buf;
        #endif
};
*/

int createSem(int key, int num);
void destroySem(int semid);
void initSem(int semid, int semnum, int val);
void semReserve(int semid, int semnum);
void semRelease(int semid, int semnum);
void loadConfig(int* numPazienti, int* numReparti, int* maxTime);
void parseConfig(char* data);
size_t fileLen(FILE *fp);
bool fileGetData(char* fileName, char** data);
bool isInt(char* c);

//genera "num" semafori aventi la stessa key passata come parametro
int createSem(int key, int num){
    int semid;
    if ((semid = semget((key_t) key, num, 0600 | IPC_CREAT | IPC_EXCL)) == -1){
        if (errno == EEXIST){ //se il semaforo è già stato creato
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

void loadConfig(int* numPazienti, int* numReparti, int* maxTempo){
    char* confData;
    if (fileGetData("hospital.conf", &confData)){
        //printf("%s\n", confData);
        //parseConfig(confData); // da aggiungere
        char* line;
        while((line = strsep(&confData, "\n"))){
            //printf("\nLinea: \"%s\"\n", line);
            //sscanf(line,"%s%*[ \t]%d", param, &asd);
            char* param;
            while((param = strsep(&line, " \t"))){
                if (*param) {
                    if (!strcmp(param, "pazienti")) {
                        while((param = strsep(&line, " \t"))){
                            if (*param && isInt(param)){
                                //printf("Pazienti: %d\n", atoi(param));
                                *numPazienti = atoi(param);
                                break;
                            }
                        }
                    } else if (!strcmp(param, "reparti")){
                        while((param = strsep(&line, " \t"))){
                            if (*param && isInt(param)){
                                //printf("Reparti: %d\n", atoi(param));
                                *numReparti = atoi(param);
                                break;
                            }
                        }
                    } else if (!strcmp(param, "tempo")){
                        while((param = strsep(&line, " \t"))){
                            if (*param && isInt(param)){
                                //printf("Tempo: %d\n", atoi(param));
                                *maxTempo = atoi(param);
                                break;
                            }
                        }
                    }
                }
            }
            
        }
        free(confData);
    } else {
        printf("hospital.conf non trovato! Applico le configurazioni di default\n");
    }
}


void parseConfig(char* data){

    //printf("%s\n", strsep(*data,"\n"));
}

// ottiene la lunghezza del file in byte
size_t fileLen(FILE *fp){
    size_t fileSize = -1;
    if (fp){
        fseek(fp, 0, SEEK_END);
        fileSize = ftell(fp);
        fseek(fp, 0, SEEK_SET);
    }
    return fileSize;
}

// crea una malloc con il contenuto del file
bool fileGetData(char* fileName, char** data){
    bool success = false;
    FILE* fp = fopen(fileName, "r");
    if (fp) {
        size_t fileSize = fileLen(fp);
        *data = (char*) malloc(sizeof(char)*fileSize+1); // +1 fix per stringhe
        fread(*data, fileSize, 1, fp);
        fclose(fp);
        (*data)[fileSize] = '\0'; // fix per stringhe
        success = true;
    }
    return success;
}

bool isInt(char* c){
    if (*c == '-' || *c == '+')
        c++;
    while(*c){
        if (!isdigit(*c++))
            return false;
    }
    return true;
}


