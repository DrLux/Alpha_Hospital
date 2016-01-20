#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/sem.h>
#include <sys/types.h>
#include <ctype.h>
#include <sys/msg.h>
#include <unistd.h>
#include <errno.h>
#include <stdbool.h>

//struttura che definisce il paziente
struct paziente {
    long mtype; /* GRAVITA Indice (da 1 a 10) di gravita della malattia del paziente */
    char *malattia; /* Nome della malattia */
}; 


struct schedaSintomo {
    char* sintomo;
    int reparto;
    int gravita;
};

struct elencoSintomi {
	int numSintomi;
	struct schedaSintomo** arraySintomi;
};

//Handler che gestisce SigQuit e SigAlarm
static void chiusuraOspedale(int sig);

//variabile globale che definisce se l' ospedale è aperto o chiuso
extern int GLOBAL_SWITCH;

