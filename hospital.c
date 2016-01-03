#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/sem.h>
#include <sys/types.h>
#include <sys/msg.h>
#include <unistd.h>
#include <errno.h>
#include "hospital.h"

#define DEFAULT_PAZIENTI 10
#define DEFAULT_REPARTI 2
#define DEFAULT_MAX_TIME 20

#define KEY 0xfaceb00c

void triage();
//void getMessage(int msgid, struct ddqueue *msg, long msgtype);
//void setMessage(int msgid, struct ddqueue *msg, long msgtype);
int createCodeMessage (int key);
void loadConfig(int* numPazienti, int* numReparti, int* maxTime);
void semRelease(int semid, int semnum);
void semReserve(int semid, int semnum);
void initSem(int semid, int semnum, int val);
int destroySem(int semid);
int createSem(int key, int num);


int main(int argc, char* argv[]){
        int numPazienti = DEFAULT_PAZIENTI,
        numReparti = DEFAULT_REPARTI,
        maxTempo = DEFAULT_MAX_TIME;
		
        loadConfig(&numPazienti, &numReparti, &maxTempo);
		
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