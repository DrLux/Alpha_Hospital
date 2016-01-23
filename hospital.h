#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/sem.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <ctype.h>
#include <sys/msg.h>
#include <unistd.h>
#include <errno.h>
#include <stdbool.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <limits.h>
#include <sys/ipc.h>
#include <signal.h>

extern bool OSPEDALE_APERTO;
extern bool OSPEDALE_IN_CHIUSURA;

//struttura che definisce il paziente
struct cliente {
    long mtype; /* non usato, mantenuto per compatibilita con message queue */
    char* sintomo; /* Nome della malattia */
}; 

struct paziente {
    long mtype; /* usato come indice priorita nelle prestazioni */
    unsigned long ID;
    int gravita;
    char* sintomo; /* Nome della malattia */
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
//static void chiusuraOspedale(int sig);

//variabile globale che definisce se l' ospedale è aperto o chiuso
//extern int GLOBAL_SWITCH;

