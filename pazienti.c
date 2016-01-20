#include "hospital.h"
#include "config.h"
#include "pazienti.h"


// genera messaggi relativi ai pazienti verso il triage
void generaPazienti(int msgqIDgp2tri, struct elencoSintomi* sintomi){
	struct paziente persona;

	int i=10;
	while(i--){
		char* sintomo = getSintomoRantom(sintomi);
		printf("%s\n", sintomo);
	}
	//sendMessage(msgqIDgp2tri, sintomo, (int)strlen(sintomo));

	//persona.malattia = (char*) malloc(sizeof());
	//int totaleRighe = contaRighe("malattie.conf");

	//loadMalattia((persona).malattia, getRand(1 , totaleRighe)); //numero random da 1 al totale delle righe (valore calcolato runtime)

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


// inserisce in un arrai di puntatori a struct i sintomi e i relativi reparti e gravita
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
					        (*localMalattiaArray[realLength]).reparto = atoi(reparto);
					        (*localMalattiaArray[realLength]).gravita = atoi(gravita);
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


// ottiene un sintomo random dalla lista su file
char* getSintomoRantom(struct elencoSintomi* sintomi){
	int maxRandom = (*sintomi).numSintomi-1;
	int random = getRand(0, maxRandom);
	char* sintomoRandom = (*(*sintomi).arraySintomi[random]).sintomo;
	char* sintomo = (char*) malloc(strlen(sintomoRandom)+1);
	strcpy(sintomo, sintomoRandom);
	return sintomo;
}



