#include "hospital.h"
#include "comm.h"
#include "triage.h"

#define FIFO_PATH_FOLDER "fifos\0"
#define FIFO_BASE_NAME "fifo\0"


#ifdef PRINT_COLOR
#define TRIAGE_NAME ANSI_COLOR_RED "Pazienti" ANSI_COLOR_RESET
#else
#define TRIAGE_NAME "Pazienti"
#endif

void triage(int semIDPazienti, int msgqPazienti, int reparti, struct elencoSintomi* sintomi){
	//printf("TRIAGE AVVIATO\n");
	printf("["TRIAGE_NAME"] AVVIATO\n");

	// creo array che conterra' i puntatori ai file aperti per ogni reparto
	int* fifoIDReparti = (int*) malloc(reparti*sizeof(int)); 

	int charsNum = (int)log10(reparti)+1; // calcolo quanti carattri servono per rappresentare il numero massimo dei reparto
	int basePathLen = strlen(FIFO_PATH_FOLDER) + 1 + strlen(FIFO_BASE_NAME) + charsNum; // fifos/fifoNN
	// creo stringa contenente path per ogni reparto
	char* fifoPathReparto = (char*) malloc((basePathLen+1)*sizeof(char)); // +1 per terminatore stringa


	int IDReparto = 0; // modifico il valore di IDReparto in createChildsWithFifo, cosi ogni reparto ha il proprio id dopo essere stato creato
	// generazione reparti e relative code messaggi
	bool isChild = createChildsWithFifo(reparti, fifoIDReparti, fifoPathReparto, &IDReparto);
	if (isChild) { // routine reparti
		reparto(fifoPathReparto, IDReparto, semIDPazienti);
		// exit del figlio
		exit(EXIT_SUCCESS);
	}



	struct cliente persona;
	struct paziente nuovoPaziente;

	// ROUTINE TRIAGE PRINCIPALE
	unsigned long idPaziente = 0; 
	while(OSPEDALE_APERTO && !OSPEDALE_IN_CHIUSURA){
		// prenso un nuovo cliente dalla coda generatore pazienti
		if (recvMessage(msgqPazienti, &persona, sizeof(persona), 0)) { 

			idPaziente++; // genero ID paziente 

			char* sintomoRicevuto = persona.sintomo; // ottengo il sintomo del cliente
			int reparto, gravita;
			getRepartoGravita(sintomi, sintomoRicevuto, &reparto, &gravita);// associo il sintomo al reparto e alla gravita corrispondente
			// PRINT INFO
			printf("["TRIAGE_NAME"] Paziente: %ld, Sintomo: %s, Reparto: %d, Gravita': %d\n", idPaziente, sintomoRicevuto, reparto, gravita);


			if (reparto <= reparti){ // se il reparto esiste invio il paziente
				nuovoPaziente.ID = idPaziente;
				nuovoPaziente.gravita = gravita;
				nuovoPaziente.sintomo = sintomoRicevuto;
				// invio sul fifo del reparto corrispondente il nuovo paziente
				write(fifoIDReparti[reparto-1], &nuovoPaziente, sizeof(struct paziente));
			} else { // se il reparto NON esiste NON servo il paziente
				printf("["TRIAGE_NAME"] Reparto: %d inesistente! NON servo il paziente %ld\n", reparto, idPaziente);
				// incremento semaforo pazienti
				semRelease(semIDPazienti, 0);
			}
			
		}
	}

	printf("["TRIAGE_NAME"] ** ATTENDO FIGLI **\n");
	waitAllChild(); // aspetto che muoiano tutti i figli prima di liberare le risorse
	printf("["TRIAGE_NAME"] ** CHIUDO **\n");


	// pulisco memoria e fifo
	removeFifos(reparti, fifoIDReparti, fifoPathReparto);
	free(fifoPathReparto);
	free(fifoIDReparti);
}


// crea tanti figli quanti sono i reparti e ad ognuno crea e passa una coda di messaggi 
bool createChildsWithFifo(int n, int* fifoids, char* repfifopath, int* repid){
	mkdir(FIFO_PATH_FOLDER, 0700); // creo la cartella che conterra' i fifo
	bool isChild = false;
	int i;
	for (i=0; i<n && !isChild; i++) {

		*repid = i+1;
		sprintf(repfifopath, "%s/%s%d", FIFO_PATH_FOLDER, FIFO_BASE_NAME, *repid); // fifos/fifoNN
		mkfifo(repfifopath, 0600); // creo i fifo
		
		if(!fork()) // genero i reparti
			isChild = true; // se non un figlio (reparto) esco e ritorno true
		else {
			fifoids[i] = open(repfifopath, O_WRONLY); // apro in scrittura i fifo e salvo i puntatori nell'array
		}
	}
	return isChild;
}

void removeFifos(int n, int* fifoids, char* repfifopath){
	int i;
	for (i=0; i<n; i++) {
		close(fifoids[i]); // chiudo i fifo (aperti in scrittura)
		sprintf(repfifopath, "%s/%s%d", FIFO_PATH_FOLDER, FIFO_BASE_NAME, i+1); // fifos/fifoNN
		unlink(repfifopath); // elimino i fifo
	}
	rmdir(FIFO_PATH_FOLDER); // elimino la cartella che conteneva i fifo
}


// ottiene il reparto e la gravita corrispondenti a un sintomo
void getRepartoGravita(struct elencoSintomi* sintomi, char* sintomoRicevuto, int* reparto, int* gravita){
	int i;
	for (i=0; i<(*sintomi).numSintomi; i++) {
		// stessa cosa se si fa la string compare o il confronto fra gli indirizzi dato che puntano alla stessa struttura nella heap
		if ((*(*sintomi).arraySintomi[i]).sintomo == sintomoRicevuto) {
			*reparto = (*(*sintomi).arraySintomi[i]).reparto; // inizializzo reparto corrispondente al sintomo
			*gravita = (*(*sintomi).arraySintomi[i]).gravita; // inizializzo gravita corrispondente al sintomo
			break;
		}
	}
}


