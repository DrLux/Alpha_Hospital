#include "hospital.h"
#include "prestazione.h"

void prestazione(int msgqIDrep2pre, int semPazienti){
	while (false){ // TRUE
		//recvMessage(msgqIDrep2pre, );
		// qua bisogna gestire starvation (oppure all'invio in reparto)
		if (false){ // se ha ricevuto un messaggio
			sleep(3); // sleep random
			semRelease(semPazienti, 0); // rilascia il semaforo
		}
	}
}
