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

// gestisce SIGQUIT
extern bool OSPEDALE_APERTO;
// gestisce SIGALRM
extern bool OSPEDALE_IN_CHIUSURA;

//struttura che definisce il paziente
struct cliente {
    long mtype; // non usato, mantenuto per compatibilita con message queue 
    char* sintomo; // Nome sintomo 
}; 

// struct usata tra triage --> reparto e reparti --> prestazioni
struct paziente {
    long mtype; // usato come indice priorita nelle prestazioni 
    unsigned long ID; // Identificativo paziente
    int gravita;
    char* sintomo; // puntatore a nome sintomo
}; 

// associazione config sintomo <--> reparto <--> gravita
// usata in pazienti (solo nome sintomo) e triage
struct schedaSintomo {
    char* sintomo; // puntatore a nome sintomo
    int reparto;
    int gravita;
};

// contenutore elenco dei sintomi
// usata in pazienti e triage
struct elencoSintomi {
	int numSintomi; // dimensione riferimento array parzialmente riempito
	struct schedaSintomo** arraySintomi; // puntatore ad array di puntatory a schedaSintomo
};

void sigquit_handler(int signum);
void sigalarm_handler(int signum);
void sigalarm_handler_propagate(int signum);

