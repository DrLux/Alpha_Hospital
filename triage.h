#include "reparto.h"

void triage(int semPazienti, int msgqPazienti, int reparti);
bool createChildsWithQueue(int n, int* msgqids, int* repmsgqid);


void triage(int semPazienti, int msgqPazienti, int reparti){

	int* msgqIDReparti = (int*) malloc(reparti*sizeof(int));
	int msgqIDReparto;

	// generazione reparti e relative code messaggi
	bool isChild = createChildsWithQueue(reparti, msgqIDReparti, &msgqIDReparto);
	if (isChild) { // routine reparti
		reparto(msgqIDReparto, semPazienti);
		exit(EXIT_SUCCESS);
	}




	// ROUTINE TRIAGE PRINCIPALE
	bool accept = false; // TRUE
	while(accept){
		// ricevo un messaggio da msgqPazienti
		if (false) { // se ho ricevuto un messaggio
			semReserve(semPazienti, 0); // decremento il semaforo dei pazienti
			// associo la malattia ad un reparto
			// invio sulla code del reparto msgqIDReparti[NUM_REPARTO] il messaggio ricevuto
		}
	}



	// pulisco memoria e code messaggi
	int i;
	for (i=0; i<reparti; i++) {
		destroyMsgQ(msgqIDReparti[i]);
	}
	free(msgqIDReparti);
}



bool createChildsWithQueue(int n, int* msgqids, int* repmsgqid){
	bool isChild = false;
	int i;
	for (i=0; i<n && !isChild; i++) {
		*repmsgqid = createMsgQ(getpid()-n+i, true);
		//printf("%d\n", *repmsgqid);
		if(!fork())
			isChild = true;
		else
			msgqids[i] = *repmsgqid;
	}
	return isChild;
}