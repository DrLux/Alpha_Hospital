#include "hospital.h"
#include "reparto.h"

void reparto(int msgqIDtri2rep, int semPazienti){
	
	int msgqIDrep2pre = createMsgQ(getpid(), false);
	int pid = fork();
	if (!pid) {
		prestazione(msgqIDrep2pre, semPazienti);
		exit(EXIT_SUCCESS);
	}


	// ROUTINE PRINCIPALE REPARTI 
	while (false) { // TRUE
		//qui rcv messaggio dal triage
		if (false){ // ---> eseguo qui dentro solo se rivevo il messaggio // se riceve un messaggio dal triage (msgqIDtri2rep)
			// lo manda alla prestazione con priorità (msgqIDrep2pre)
			// qua bisogna gesitre starvation (oppure alla ricezione in prestazione)
		}
		sleep(1); // a intervalli regolari
	}

	destroyMsgQ(msgqIDrep2pre);
}
