#include "hospital.h"
#include "reparto.h"
#include "comm.h"


void reparto(char* fifoPathTriage, int IDReparto, int semPazienti){

	printf("REPARTO %d AVVIATO\n", IDReparto);

	
	int fifoIDTriage = open(fifoPathTriage, O_RDONLY | O_NONBLOCK);
	int msgqIDPrestazione = createMsgQ(getpid(), false);

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
			
			/*
			if (recvMessage(msgqIDtri2rep, paz_rep, 0)) //se ci sono pazienti in lista di attesa
				sendMessage(msgqIDrep2pre, paz_rep, (int)strlen((*paz_rep).malattia));// lo manda alla prestazione con priorità (msgqIDrep2pre)
			else	if (GLOBAL_SWITCH == 1) //controlla che l' ospedale sia chiuso
						open = false;
			*/
			sleep(1); // a intervalli regolari
		} 
	}

	printf("[Reparto %d] ** ATTENDO FIGLI **\n", IDReparto);
	waitAllChild();
	printf("[Reparto %d] ** CHIUDO **\n", IDReparto);
	
	destroyMsgQ(msgqIDPrestazione);
	close(fifoIDTriage);
}
