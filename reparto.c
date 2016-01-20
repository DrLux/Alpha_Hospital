#include "hospital.h"
#include "reparto.h"
#include "comm.h"

void reparto(int msgqIDtri2rep, int semPazienti){
	
	int msgqIDrep2pre = createMsgQ(getpid(), false);
	int pid = fork();
	if (!pid) {
		prestazione(msgqIDrep2pre, semPazienti);
		exit(EXIT_SUCCESS);
	}

	
	// ROUTINE PRINCIPALE REPARTI 
	struct paziente *paz_rep = malloc (sizeof(struct paziente));//qui fare la malloc
	bool open = true;
	/*
	while (open && GLOBAL_SWITCH != 2) { 
		if (recvMessage(msgqIDtri2rep, paz_rep, 0)) //se ci sono pazienti in lista di attesa
			sendMessage(msgqIDrep2pre, paz_rep, (int)strlen((*paz_rep).malattia));// lo manda alla prestazione con priorità (msgqIDrep2pre)
		else	if (GLOBAL_SWITCH == 1) //controlla che l' ospedale sia chiuso
					open = false;
		sleep(1); // a intervalli regolari
	}
	*/
	destroyMsgQ(msgqIDrep2pre);
}
