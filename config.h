void loadConfig(int* numPazienti, int* numReparti, int* maxTime);
void parseConfig(char* data);
size_t fileLen(FILE *fp);
bool fileGetData(char* fileName, char** data);
bool isInt(char* c);

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