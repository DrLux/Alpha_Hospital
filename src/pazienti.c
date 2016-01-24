#include "hospital.h"
#include "config.h"
#include "comm.h"
#include "pazienti.h"

#ifdef PRINT_COLOR
#define PAZIENTI_NAME ANSI_COLOR_YELLOW "Pazienti" ANSI_COLOR_RESET 
#else
#define PAZIENTI_NAME "Pazienti"
#endif

// genera messaggi relativi ai pazienti verso il triage
void generaPazienti(int semIDPazienti, int msgqIDTriage, struct elencoSintomi* sintomi){
	printf("["PAZIENTI_NAME"] AVVIATO\n");


	struct cliente persona;
	persona.mtype = 1; // anche se uso la coda come FIFO, mtype deve essere maggiore di 0

	unsigned long i=0; // contatore clienti generati
	while(OSPEDALE_APERTO && !OSPEDALE_IN_CHIUSURA){
		// decremento il semaforo dei pazienti
		semReserve(semIDPazienti, 0); 
		if (OSPEDALE_APERTO && !OSPEDALE_IN_CHIUSURA) {

			persona.sintomo = getSintomoRandom(sintomi); // ottiene sintomo random e lo inizializza nel nuovo cliente
			printf("["PAZIENTI_NAME"] Genero paziente %ld con sintomo: %s\n", i, persona.sintomo); // PRINT INFO
			sendMessage(msgqIDTriage, &persona, sizeof(persona)); // invia al triage il nuovo cliente
			i++;
		}
	}

	printf("["PAZIENTI_NAME"] ** CHIUDO **\n");

}

// carica i sintomi da file in memoria
void loadSintomi(struct elencoSintomi** sintomi, char* basePathConf){
	char* sintomiPath;
	if (basePathConf!=NULL)
		sintomiPath = makePath(basePathConf, SINTOMI_CONF);
	else
		sintomiPath = makePath(DEFAULT_CONF_DIR, SINTOMI_CONF);

    char* sintomiData;
    if ( fileGetData(sintomiPath, &sintomiData) ){
        parseSintomi(sintomiData, sintomi);
        free(sintomiData);
    } else {
        printf("[ERRORE] sintomi.conf non trovato! Termino\n");
        exit(EXIT_FAILURE);
    }
    free(sintomiPath);
}


// inserisce in un array di puntatori a struct i sintomi e i relativi reparti e gravita
void parseSintomi(char* data, struct elencoSintomi** sintomi){
	
	int nLine = countLine(data);
	struct schedaSintomo** sintomiArray = (struct schedaSintomo**) malloc(nLine*sizeof(struct schedaSintomo*));

	int i;
	int realLength = 0;
	for (i=0; i<nLine; i++) {

		char* line = strsep(&data, "\n"); // prendo una linea
		if (*line){

			char* sintomo = strsep(&line, ","); // prendo il testo fino alla prima virgola
			if (sintomo && *sintomo && line && *line) {

				char* secondSubString = strsep(&line, ","); // prendo il testo fino alla seconda virgola
				char* reparto = getNextLexeme(&secondSubString); // prendo la prima stringa che trovo (ignorando tab e spazi)
				if (reparto && *reparto && line && *line){

					char* gravita = getNextLexeme(&line); // su cio che rimane della riga dopo la seconda virgola prendo la prima stringa che trovo (ignorando tab e spazi)
					if (gravita && *gravita){

						if (isInt(reparto) && isInt(gravita)){ // se entrambe le stringhe (dopo la prima e la seconda virgola) sono interi
							sintomiArray[realLength] = (struct schedaSintomo*) malloc(sizeof(struct schedaSintomo)); // creo una struttura schedaSintomo (sintomo, reparto, gravita)
							(*sintomiArray[realLength]).sintomo = (char*) malloc(strlen(sintomo)+1); // +1 per terminatore stringa
					        strcpy((*sintomiArray[realLength]).sintomo, sintomo); // inizializzo schedaSintomo.nomeSintomo
					        (*sintomiArray[realLength]).reparto = rangeRestrict(atoi(reparto), 1, 10); // inizializzo schedaSintomo.reparto
					        (*sintomiArray[realLength]).gravita = rangeRestrict(atoi(gravita), 1, 10); // inizializzo schedaSintomo.gravita
							realLength++; // incremendo la dimensione di riferimento dell'array (parzialmente riempito in caso alcune righe del file sintomi.conf siano corrotte)
						}
					} 
				}
			}
		}
	}

	*sintomi = (struct elencoSintomi*) malloc(sizeof(struct elencoSintomi)); // genero il contenutore dei sintomi
	(*sintomi)->numSintomi = realLength; // inizializzo la dimensione di riferimento
	(*sintomi)->arraySintomi = sintomiArray; // aggancio al puntatore dell'elenco dei sintomi l'array creato nella heap

	//for (i=0; i<realLength; i++) {
	//	printf("[Parsing] %s, %d, %d\n", (*sintomiArray[i]).sintomo, (*sintomiArray[i]).reparto, (*sintomiArray[i]).gravita);
	//}
}


// ottiene un sintomo random dall'elenco dei sintomi
char* getSintomoRandom(struct elencoSintomi* sintomi){
	int maxRandom = (*sintomi).numSintomi-1;
	int random = getRand(0, maxRandom);
	char* sintomoRandom = (*(*sintomi).arraySintomi[random]).sintomo;
	return sintomoRandom;
}


//conta il numero di righe in un testo
int countLine(char *dataPointer){
	int counter = 1;
	while(*dataPointer){
		if (*dataPointer++ == '\n')
			counter++;
	}
	return counter;
}

// forza il numero passato al range stabilito
int rangeRestrict(int num, int min, int max){
    return (num>max)? max : ((num<min)? min: num);
}


