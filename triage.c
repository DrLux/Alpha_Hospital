#include "hospital.h"
#include "triage.h"

void triage(int semPazienti, int msgqPazienti, int reparti, struct elencoSintomi* sintomi){

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
		// ricevo un messaggio da msgqPazienti --| recive fifo con tipe 0
		if (false) { // se ho ricevuto un messaggio ---> ignora false
			semReserve(semPazienti, 0); // decremento il semaforo dei pazienti
			char* sintomoRicevuto = "TEMP"; // AGGIORNARE CON MESSAGGIO RICEVUTO DALLA CODA
			int reparto, gravita;
			// associo il sintomo al reparto e alla gravita corrispondente
			getRepartoGravita(sintomi, sintomoRicevuto, &reparto, &gravita);
			// invio sulla code del reparto msgqIDReparti[NUM_REPARTO] il messaggio ricevuto
			// sendFifo -> reparto, priorita 
		}
	}



	// pulisco memoria e code messaggi // DA RIFARE CON FIFO
	int i;
	for (i=0; i<reparti; i++) {
		destroyMsgQ(msgqIDReparti[i]);
	}
	free(msgqIDReparti);
}


// crea tanti figli quanti sono i reparti e ad ognuno crea e passa una coda di messaggi // DA RIFARE CON FIFO
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

// ottiene il reparto e la gravita corrispondenti a un sintomo
void getRepartoGravita(struct elencoSintomi* sintomi, char* sintomoRicevuto, int* reparto, int* gravita){
	int i;
	for (i=0; i<(*sintomi).numSintomi; i++) {
		if (!strcmp((*(*sintomi).arraySintomi[i]).sintomo, sintomoRicevuto)) {
			*reparto = (*(*sintomi).arraySintomi[i]).reparto;
			*gravita = (*(*sintomi).arraySintomi[i]).gravita;
			break;
		}
	}
}


