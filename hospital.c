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
//#include "generatorePazienti.h"
//#include "cartellaPaziente.h"

#define DEFAULT_PAZIENTI 10
#define DEFAULT_REPARTI 2
#define DEFAULT_TEMPO 20

#define KEY 0xfaceb00c


int main(int argc, char* argv[]){

    // inizializzazione di default delle variabili
    int numPazienti = DEFAULT_PAZIENTI,
    numReparti = DEFAULT_REPARTI,
    maxTempo = DEFAULT_TEMPO;
    // reinizializzazione delle variabili dal file di configurazione
    loadConfig(&numPazienti, &numReparti, &maxTempo);

    printf("Pazienti: %d\n", numPazienti);
    printf("Reparti: %d\n", numReparti);
    printf("Tempo: %d\n", maxTempo);

    int semid = createSem(KEY, 1);

    initSem(semid, 0, numPazienti);

    //GeneratorePazienti(numPazienti,maxTempo, KEY ,PERMESSI);
    //triage();



    destroySem(semid);
}

