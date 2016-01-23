#include "hospital.h"
#include "comm.h"
#include "reparto.h"


void reparto(char* fifoPathTriage, int IDReparto, int semIDPazienti){

	printf("REPARTO %d AVVIATO\n", IDReparto);

	
	int fifoIDTriage = open(fifoPathTriage, O_RDONLY | O_NONBLOCK); // apro il file fifo in lettura (connessione triage --> reparto)
	int msgqIDPrestazione = createMsgQ(getpid(), false); // creo la coda verso la prestazione


	// creo la prestazione
	int pid = fork();
	if (!pid) {
		prestazione(msgqIDPrestazione, IDReparto, semIDPazienti);
		exit(EXIT_SUCCESS);
	}

	// contenitore per info in arrivo dal FIFO del triage
	struct paziente pazienteDaServire;

	// ROUTINE PRINCIPALE REPARTI 
	while (OSPEDALE_APERTO) { 
		// prelevo un paziente dal FIFO del triage
		if(read(fifoIDTriage, &pazienteDaServire, sizeof(struct paziente)) > 0){
			pazienteDaServire.mtype = (long) 11-pazienteDaServire.gravita; // assegno turno (ribalto la gravita per inserire correttamente la priorita dei pazienti nella coda)
			// PRINT INFO
			printf("[Reparto %d] Paziente: %ld, Sintomo: %s, Gravita: %d\n", IDReparto, pazienteDaServire.ID, pazienteDaServire.sintomo, pazienteDaServire.gravita);
			sendMessage(msgqIDPrestazione, &pazienteDaServire, sizeof(pazienteDaServire));// invio alla prestazione il cliente da operare
			sleep(1); // processo intervalli regolari
		} else {
			if (OSPEDALE_IN_CHIUSURA) // ricevuta SIGALRM, se non ho piu nessun paziente da servire termino
				OSPEDALE_APERTO = false;
		}
	}

	printf("[Reparto %d] ** ATTENDO FIGLI **\n", IDReparto);
	waitAllChild(); // aspetto che muoiano tutti i figli prima di liberare le risorse
	printf("[Reparto %d] ** CHIUDO **\n", IDReparto);
	
	destroyMsgQ(msgqIDPrestazione); // distruggo la coda verso la prestazione
	close(fifoIDTriage); // chiudo FIFO in lettura
}
