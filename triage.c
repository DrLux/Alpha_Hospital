#include "hospital.h"
#include "comm.h"
#include "triage.h"
#include <math.h>

#define FIFO_PATH_FOLDER "fifos\0"
#define FIFO_BASE_NAME "fifo\0"


void triage(int semPazienti, int msgqPazienti, int reparti, struct elencoSintomi* sintomi){
	printf("TRIAGE AVVIATO\n");

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
		reparto(fifoPathReparto, IDReparto, semPazienti);
		// exit del figlio
		exit(EXIT_SUCCESS);
	}



	struct cliente persona;
	struct paziente nuovoPaziente;
	nuovoPaziente.mtype = 1;

	// ROUTINE TRIAGE PRINCIPALE
	unsigned long idPaziente = 0; 
	//while(OSPEDALE_APERTO){
	while(OSPEDALE_APERTO && !OSPEDALE_IN_CHIUSURA){
		if (recvMessage(msgqPazienti, &persona, sizeof(persona), 0)) { // se ho ricevuto un messaggio

			idPaziente++; // gestire caso in cui superi il limite massimo

			char* sintomoRicevuto = persona.sintomo; 
			int reparto, gravita;
			// associo il sintomo al reparto e alla gravita corrispondente
			getRepartoGravita(sintomi, sintomoRicevuto, &reparto, &gravita);
			printf("[Triage] Paziente: %ld, Sintomo: %s, Reparto: %d, Gravita': %d\n", idPaziente, sintomoRicevuto, reparto, gravita);

			if (reparto <= reparti){
				// invio sulla code del reparto msgqIDReparti[NUM_REPARTO] il messaggio ricevuto
				// sendFifo -> reparto, priorita 
				nuovoPaziente.ID = idPaziente;
				nuovoPaziente.gravita = gravita;
				nuovoPaziente.sintomo = sintomoRicevuto;
				write(fifoIDReparti[reparto-1], &nuovoPaziente, sizeof(struct paziente));
			} else {
				printf("[Triage] Reparto: %d inesistente! NON servo il paziente\n", reparto);
				semRelease(semPazienti, 0);
			}
			
		}
	}

	printf("[Triage] ** ATTENDO FIGLI **\n");
	waitAllChild();
	printf("[Triage] ** CHIUDO **\n");


	// pulisco memoria e fifo
	removeFifos(reparti, fifoIDReparti, fifoPathReparto);
	free(fifoPathReparto);
	free(fifoIDReparti);
}


// crea tanti figli quanti sono i reparti e ad ognuno crea e passa una coda di messaggi 
bool createChildsWithFifo(int n, int* fifoids, char* repfifopath, int* repid){
	mkdir(FIFO_PATH_FOLDER, 0700);
	bool isChild = false;
	int i;
	for (i=0; i<n && !isChild; i++) {

		*repid = i+1;
		sprintf(repfifopath, "%s/%s%d", FIFO_PATH_FOLDER, FIFO_BASE_NAME, *repid); // fifos/fifoNN
		//printf("%s\n", repfifopath); // commentare
		mkfifo(repfifopath, 0600);
		
		if(!fork())
			isChild = true;
		else {
			fifoids[i] = open(repfifopath, O_WRONLY);
		}
	}
	return isChild;
}

void removeFifos(int n, int* fifoids, char* repfifopath){
	int i;
	for (i=0; i<n; i++) {
		close(fifoids[i]);
		sprintf(repfifopath, "%s/%s%d", FIFO_PATH_FOLDER, FIFO_BASE_NAME, i+1); // fifos/fifoNN
		unlink(repfifopath);
	}
	rmdir(FIFO_PATH_FOLDER);
}


// ottiene il reparto e la gravita corrispondenti a un sintomo
void getRepartoGravita(struct elencoSintomi* sintomi, char* sintomoRicevuto, int* reparto, int* gravita){
	int i;
	for (i=0; i<(*sintomi).numSintomi; i++) {
		// stessa cosa se si fa la string compare o il confronto fra gli indirizzi dato che puntano alla stessa struttura nella heap
		//if (!strcmp((*(*sintomi).arraySintomi[i]).sintomo, sintomoRicevuto)) { 
		if ((*(*sintomi).arraySintomi[i]).sintomo == sintomoRicevuto) {
			*reparto = (*(*sintomi).arraySintomi[i]).reparto;
			*gravita = (*(*sintomi).arraySintomi[i]).gravita;
			break;
		}
	}
}


