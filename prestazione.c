#include "hospital.h"
#include "comm.h"
#include "prestazione.h"


void prestazione(int msgqIDrep2pre, int IDPrestazione, int semPazienti){
	printf("PRESTAZIONE %d AVVIATO\n", IDPrestazione);
	srand(getpid());
	struct paziente pazienteDaOperare;

	int starvation = 0, priority;
	while(OSPEDALE_APERTO){
		if (starvation > 9){
			printf("[Prestazione %d] Prendo primo in coda (gestione starvation)\n", IDPrestazione);
			priority = 0;
			starvation = 0;
		} else {
			priority = -11;
		}

		if (recvMessage(msgqIDrep2pre, &pazienteDaOperare, sizeof(pazienteDaOperare), priority)) {
			starvation++;
			printf("[Prestazione %d] OPERO Paziente: %ld, Sintomo: %s, Gravita: %d\n", IDPrestazione, pazienteDaOperare.ID, pazienteDaOperare.sintomo, pazienteDaOperare.gravita);
			int randomTime = getRand(1,5);
			//printf("\nTEMPO: %d\n", randomTime);
			sleep(randomTime); 
			semRelease(semPazienti, 0);
		} else {
			starvation = 0; // se non c'e' nessun paziente in coda resetto la gestione della starvation
		}
	}

	printf("[Prestazione %d] ** CHIUDO ** \n", IDPrestazione);

}
