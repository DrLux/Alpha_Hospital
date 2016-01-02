#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/sem.h>
#include <sys/types.h>
#include <unistd.h>
#include <errno.h>
#include "hospital.h"

#define DEFAULT_PAZIENTI 10
#define DEFAULT_REPARTI 2
#define DEFAULT_MAX_TIME 20

int main(int argc, char* argv[]){
        int numPazienti = DEFAULT_PAZIENTI,
        numReparti = DEFAULT_REPARTI,
        maxTempo = DEFAULT_MAX_TIME;

        loadConfig(&numPazienti, &numReparti, &maxTempo);

        int semid = createSem(0xfaceb00c, 1);
        initSem(semid, 0, 10);

        



        destroySem(semid);
}