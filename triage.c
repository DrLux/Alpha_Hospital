#include "hospital.h"
#include "triage.h"


void triage(int semPazienti, int msgqPazienti, int reparti, struct elencoSintomi* sintomi){
	printf("TRIAGE AVVIATO\n");


	int* fifoIDReparti = (int*) malloc(reparti*sizeof(int));
	char fifoPathReparto[] = "fifos/fifo00\0";
	int IDReparto = 0;

	// generazione reparti e relative code messaggi
	bool isChild = createChildsWithFifo(reparti, fifoIDReparti, fifoPathReparto, &IDReparto);
	if (isChild) { // routine reparti
		reparto(fifoPathReparto, IDReparto, semPazienti);
		// exit del figlio
		exit(EXIT_SUCCESS);
	}



	struct cliente persona;
	struct paziente nuovoPaziente;
	nuovoPaziente.turno = 1;

	// ROUTINE TRIAGE PRINCIPALE
	unsigned long idPaziente = 0; 
	while(OSPEDALE_APERTO){
		if (recvMessage(msgqPazienti, &persona, sizeof(persona), 0)) { // se ho ricevuto un messaggio

			idPaziente++; // gestire caso in cui superi il limite massimo

			//printf("ATTENDO PERMESSO\n");
			
			//semReserve(semPazienti, 0); // decremento il semaforo dei pazienti
			//if (!OSPEDALE_APERTO) break;

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


	// pulisco memoria e code messaggi // DA RIFARE CON FIFO
	removeFifos(reparti, fifoIDReparti, fifoPathReparto);
	free(fifoIDReparti);
}


// crea tanti figli quanti sono i reparti e ad ognuno crea e passa una coda di messaggi // DA RIFARE CON FIFO
bool createChildsWithFifo(int n, int* fifoids, char* repfifopath, int* repid){
	mkdir("fifos", 0700);
	bool isChild = false;
	int i;
	for (i=0; i<n && !isChild; i++) {

		*repid = i+1;
		sprintf(repfifopath, "fifos/fifo%d", *repid);
		//printf("%s\n", repfifopath);
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
		sprintf(repfifopath, "fifos/fifo%d", i+1);
		unlink(repfifopath);
	}
	rmdir("fifos");
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


