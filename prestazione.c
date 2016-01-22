#include "hospital.h"
#include "prestazione.h"


void prestazione(int msgqIDrep2pre, int IDPrestazione, int semPazienti){
	printf("PRESTAZIONE %d AVVIATO\n", IDPrestazione);

	//printf("[PRESTAZIONE %d] id coda: %d\n", IDPrestazione, msgqIDrep2pre);

	struct paziente pazienteDaOperare;

	while(OSPEDALE_APERTO){
		if (recvMessage(msgqIDrep2pre, &pazienteDaOperare, sizeof(pazienteDaOperare), 0)) {
			printf("[Prestazione %d] OPERO Paziente: %ld, Sintomo: %s, Gravita: %d\n", IDPrestazione, pazienteDaOperare.ID, pazienteDaOperare.sintomo, pazienteDaOperare.gravita);

			sleep(5); // RANDOM
			semRelease(semPazienti, 0);
		} 
	}

	printf("[Prestazione %d] ** CHIUDO **\n", IDPrestazione);

}
