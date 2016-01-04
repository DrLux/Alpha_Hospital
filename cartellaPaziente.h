void malattiaPaziente(struct paziente *msg);

//deve dare a ogni paziente una malattia scelta a caso dal tzt
void malattiaPaziente(struct paziente *msg){
	int length_malattia = 10; //ESEMPIO
	(*msg).reparto = 0;
	(*msg).mtype = 0;
	(*msg).malattia =(char *) malloc(sizeof(char) * length_malattia);
	stpcpy((*msg).malattia,"malanno");
}

/*void loadMalattia(struct paziente *msg){
    char* malattiaData;
    if (fileGetData("malattie.txt", &confData)){
        char* line;
        while((line = strsep(&confData, "\n"))){
            char* param;
            while((param = strsep(&line, " \t"))){
                if (*param) {
                    if (!strcmp(param, "pazienti")) 
                        while((param = strsep(&line, " \t"))){
                            if (*param && !isInt(param)){
                                //printf("Pazienti: %d\n", atoi(param));
                                *numPazienti = atoi(param);
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
	*/
