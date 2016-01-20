#include "hospital.h"
#include "prestazione.h"

void prestazione(int msgqIDrep2pre, int semPazienti){
	struct paziente *paz_pre = malloc (sizeof(struct paziente));//qui fare la malloc
	bool open = true;
	/*
	while (open && GLOBAL_SWITCH != 2){//GLOBAL_SWITCH viene settata a 2 in caso di sigquit 
		if (recvMessage(msgqIDrep2pre, paz_pre, -10)){ //opero il paziente che ricevo
			// qua bisogna gestire starvation (oppure all'invio in reparto)
			sleep(getRand(1,10)); // sleep random
			semRelease(semPazienti, 0); // rilascia il semaforo
		} else if (GLOBAL_SWITCH == 1) //se l' ospedale è chiuso e non ci sono altri pazienti da operare
			open = false; //break per uscire dal ciclo	
	} //se non ci sono pazienti ma l' ospedale è ancora aperto, continuo a ciclare
	*/
	free(paz_pre);
	//semRelease(semPazienti, 0);
}
