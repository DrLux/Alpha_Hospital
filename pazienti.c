#include "hospital.h"
#include "config.h"
#include "pazienti.h"
#include "comm.h"

// genera messaggi relativi ai pazienti verso il triage
void generaPazienti(int semPazienti, int msgqIDgp2tri, struct elencoSintomi* sintomi){
	printf("PAZIENTI AVVIATO (sem: %d)\n", semPazienti);


	struct cliente persona;
	persona.mtype = 1;

	int i=0;
	while(OSPEDALE_APERTO){
		//semReserve(semPazienti, 0);
		semReserve(semPazienti, 0); // decremento il semaforo dei pazienti
		if (!OSPEDALE_APERTO) break;

		persona.sintomo = getSintomoRandom(sintomi);
		printf("[Pazienti %d] %s\n", i, persona.sintomo); // scommenta per vedere pazienti generati
		sendMessage(msgqIDgp2tri, &persona, sizeof(persona));
		i++;
	}

/*
	bool genereteNewClient = true;
	while(OSPEDALE_APERTO){
		if (genereteNewClient){
			persona.sintomo = getSintomoRantom(sintomi);
			printf("[Pazienti %d] %s\n", i, persona.sintomo); // scommenta per vedere pazienti generati
		}

		if( sendMessage(msgqIDgp2tri, &persona, sizeof(persona)) ) {
			genereteNewClient = true;
			i++;
		} else {
			genereteNewClient = false;
		}
	}
	*/

	printf("[Pazienti] ** CHIUDO **\n");

}

// carica i sintomi da file in memoria
void loadSintomi(struct elencoSintomi** sintomi){
    char* sintomiData;
    if ( fileGetData("malattie.conf", &sintomiData) ){
        parseSintomi(sintomiData, sintomi);
        free(sintomiData);
    } else {
        printf("malattie.conf non trovato! Termino\n");
        exit(EXIT_FAILURE);
    }
}


// inserisce in un array di puntatori a struct i sintomi e i relativi reparti e gravita
void parseSintomi(char* data, struct elencoSintomi** sintomi){
	
	int nLine = countLine(data);
	struct schedaSintomo** localMalattiaArray = (struct schedaSintomo**) malloc(nLine*sizeof(struct schedaSintomo*));

	int i;
	int realLength = 0;
	for (i=0; i<nLine; i++) {
		char* line = strsep(&data, "\n");
		if (*line){

			char* sintomo = strsep(&line, ",");
			//char* malattiaName = getNextLexeme(&line);
			if (sintomo && *sintomo && line && *line) {

				char* secondSubString = strsep(&line, ",");
				char* reparto = getNextLexeme(&secondSubString);
				if (reparto && *reparto && line && *line){

					char* gravita = getNextLexeme(&line);
					if (gravita && *gravita){

						if (isInt(reparto) && isInt(gravita)){
							localMalattiaArray[realLength] = (struct schedaSintomo*) malloc(sizeof(struct schedaSintomo));
							(*localMalattiaArray[realLength]).sintomo = (char*) malloc(strlen(sintomo)+1); // +1 per terminatore stringa
					        strcpy((*localMalattiaArray[realLength]).sintomo, sintomo);
					        (*localMalattiaArray[realLength]).reparto = rangeRestrict(atoi(reparto), 1, 10);
					        (*localMalattiaArray[realLength]).gravita = rangeRestrict(atoi(gravita), 1, 10);
							realLength++;
						}
					} 
				}
			}
		}
	}

	*sintomi = (struct elencoSintomi*) malloc(sizeof(struct elencoSintomi));
	(*sintomi)->numSintomi = realLength;
	(*sintomi)->arraySintomi = localMalattiaArray;

	//for (i=0; i<realLength; i++) {
	//	printf("[Parsing] %s, %d, %d\n", (*localMalattiaArray[i]).sintomo, (*localMalattiaArray[i]).reparto, (*localMalattiaArray[i]).gravita);
	//}
}


//conta il numero di righe in una stringa
int countLine(char *dataPointer){
	int counter = 1;
	while(*dataPointer){
		if (*dataPointer++ == '\n')
			counter++;
	}
	return counter;
}

int rangeRestrict(int num, int min, int max){
    return (num>max)? max : ((num<min)? min: num);
}


// ottiene un sintomo random dalla lista su file
char* getSintomoRandom(struct elencoSintomi* sintomi){
	int maxRandom = (*sintomi).numSintomi-1;
	int random = getRand(0, maxRandom);
	char* sintomoRandom = (*(*sintomi).arraySintomi[random]).sintomo;
	//char* sintomo = (char*) malloc(sizeof(char)*strlen(sintomoRandom)+1);
	//strcpy(sintomo, sintomoRandom);
	//return sintomo;
	return sintomoRandom;
}


