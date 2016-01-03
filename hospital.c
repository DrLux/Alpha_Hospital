#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/sem.h>
#include <sys/types.h>
#include <ctype.h>
#include <sys/msg.h>
#include <unistd.h>
#include <errno.h>
#include "hospital.h"
#include "config.h"

#define DEFAULT_PAZIENTI 10
#define DEFAULT_REPARTI 2
#define DEFAULT_TEMPO 20

#define KEY 0xfaceb00c




int main(int argc, char* argv[]){
        int numPazienti = DEFAULT_PAZIENTI,
        numReparti = DEFAULT_REPARTI,
        maxTempo = DEFAULT_TEMPO;

        loadConfig(&numPazienti, &numReparti, &maxTempo);
        printf("Pazienti: %d\n", numPazienti);
        printf("Reparti: %d\n", numReparti);
        printf("Tempo: %d\n", maxTempo);

        int semid = createSem(KEY, 1);

        initSem(semid, 0, 10);

        triage();
		

		
        destroySem(semid);
}

void triage(){
	int msgid = createCodeMessage(KEY);
	struct paziente msg;
	msg.malattia = (char *) malloc(5 * sizeof(char));
	stpcpy(msg.malattia , "lupus");
	msg.gravita = 8;
	msg.reparto = 2;
	//setMessage(msgid, &msg, IPC_NOWAIT);
}

/*
Chirurgia
Oncologia
Pediatria
Dermatologia
Cardiologia
Radiografia
Pronto Soccorso
Oculistica 
Cardiologia
*/