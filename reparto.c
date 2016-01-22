#include "hospital.h"
#include "comm.h"
#include "reparto.h"


void reparto(char* fifoPathTriage, int IDReparto, int semPazienti){

	printf("REPARTO %d AVVIATO\n", IDReparto);

	
	int fifoIDTriage = open(fifoPathTriage, O_RDONLY | O_NONBLOCK);
	int msgqIDPrestazione = createMsgQ(getpid(), false);

	//printf("[REPARTO %d] id coda: %d\n", IDReparto, msgqIDPrestazione);

	int pid = fork();
	if (!pid) {
		prestazione(msgqIDPrestazione, IDReparto, semPazienti);
		exit(EXIT_SUCCESS);
	}

	struct paziente pazienteDaServire;

	// ROUTINE PRINCIPALE REPARTI 
	while (OSPEDALE_APERTO) { 
		if(read(fifoIDTriage, &pazienteDaServire, sizeof(struct paziente)) > 0){
			pazienteDaServire.mtype = (long) 11-pazienteDaServire.gravita;
			printf("[Reparto %d] Paziente: %ld, Sintomo: %s, Gravita: %d\n", IDReparto, pazienteDaServire.ID, pazienteDaServire.sintomo, pazienteDaServire.gravita);
			sendMessage(msgqIDPrestazione, &pazienteDaServire, sizeof(pazienteDaServire));
			sleep(1); // a intervalli regolari
		} 
	}

	printf("[Reparto %d] ** ATTENDO FIGLI **\n", IDReparto);
	waitAllChild();
	printf("[Reparto %d] ** CHIUDO **\n", IDReparto);
	
	destroyMsgQ(msgqIDPrestazione);
	close(fifoIDTriage);
}
