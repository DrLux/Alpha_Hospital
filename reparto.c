#include "hospital.h"
#include "reparto.h"
#include "comm.h"


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

	
	// ROUTINE PRINCIPALE REPARTI 
	struct paziente pazienteDaServire;
	//bool open = true;
	
	while (OSPEDALE_APERTO) { 

		if(read(fifoIDTriage, &pazienteDaServire, sizeof(struct paziente)) > 0){

			printf("[Reparto %d] Paziente: %ld, Sintomo: %s, Gravita: %d\n", IDReparto, pazienteDaServire.ID, pazienteDaServire.sintomo, pazienteDaServire.gravita);

			printf("++++ RICEVUTO ++++ %ld\n", pazienteDaServire.turno);
			sendMessage(msgqIDPrestazione, &pazienteDaServire, sizeof(pazienteDaServire));
			printf("++++ INVIATO ++++\n");
			sleep(1); // a intervalli regolari
		} 
	}

	printf("[Reparto %d] ** ATTENDO FIGLI **\n", IDReparto);
	waitAllChild();
	printf("[Reparto %d] ** CHIUDO **\n", IDReparto);
	
	destroyMsgQ(msgqIDPrestazione);
	close(fifoIDTriage);
}
