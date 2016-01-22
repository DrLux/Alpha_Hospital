#include "hospital.h"
#include "comm.h"
#include "prestazione.h"


void prestazione(int msgqIDrep2pre, int IDPrestazione, int semPazienti){
	printf("PRESTAZIONE %d AVVIATO\n", IDPrestazione);
	srand(getpid());
	struct paziente pazienteDaOperare;

	while(OSPEDALE_APERTO){
		if (recvMessage(msgqIDrep2pre, &pazienteDaOperare, 35, 0)) {
			printf("[Prestazione %d] OPERO Paziente: %ld, Sintomo: %s, Gravita: %d\n", IDPrestazione, pazienteDaOperare.ID, pazienteDaOperare.sintomo, pazienteDaOperare.gravita);
			int randomTime = getRand(1, 10);
			//printf("\nTEMPO: %d\n", randomTime);
			sleep(randomTime); 
			semRelease(semPazienti, 0);
		} 
	}

	printf("[Prestazione %d] ** CHIUDO ** %d \n", IDPrestazione, semPazienti);

}
