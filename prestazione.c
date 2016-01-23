#include "hospital.h"
#include "comm.h"
#include "prestazione.h"


void prestazione(int msgqIDReparto, int IDPrestazione, int semIDPazienti){
	printf("PRESTAZIONE %d AVVIATO\n", IDPrestazione);
	srand(getpid());

	// contenitore per info in arrivo dalla coda del reparto
	struct paziente pazienteDaOperare;

	int starvation = 0, priority;
	while(OSPEDALE_APERTO){
		if (starvation > 9){ // se c'e' qualche paziente che sta attendendo piu di 10 prestazioni, prendo quello entrato meno di recente
			printf("[Prestazione %d] Prendo primo in coda (gestione starvation)\n", IDPrestazione);
			priority = 0; // mtype a 0 per prelevare il primo (come fosse una coda FIFO)
			starvation = 0; // resetto il contatore delle prestazioni effettuate
		} else {
			priority = -11; // prelevo in ordine di mtype piu' basso
		}

		// prelevo un paziente dalla coda del reparto
		if (recvMessage(msgqIDReparto, &pazienteDaOperare, sizeof(pazienteDaOperare), priority)) {
			starvation++; // incremento il contatore delle prestazioni effettuate
			int randomTime = getRand(1,5); // secondi random per operare il paziente
			// PRINT INFO
			printf("[Prestazione %d] OPERO Paziente: %ld, Sintomo: %s, Gravita: %d (Secondi: %d)\n", IDPrestazione, pazienteDaOperare.ID, pazienteDaOperare.sintomo, pazienteDaOperare.gravita, randomTime);
			sleep(randomTime); // attendo i secondi per operare
			semRelease(semIDPazienti, 0); // rilasio il semaforo per accettare un nuovo cliente
		} else {
			starvation = 0; // se non c'e' nessun paziente in coda resetto la gestione della starvation (nessun paziente sta attendendo un tempo indeterminato)
			
			if (OSPEDALE_IN_CHIUSURA) // ricevuta SIGALRM, se non ho piu nessun paziente da servire termino
				OSPEDALE_APERTO = false;
		}
	}

	printf("[Prestazione %d] ** CHIUDO ** \n", IDPrestazione);

}
