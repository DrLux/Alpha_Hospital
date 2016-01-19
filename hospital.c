#include "hospital.h"
#include "config.h"
#include "comm.h"
//#include "triage.h"
#include "generatorePazienti.h"
//#include "cartellaPaziente.h"





#define DEFAULT_PAZIENTI 10
#define DEFAULT_REPARTI 2
#define DEFAULT_TEMPO 20

#define KEY_SEM_PAZIENTI 0xfaceb00c
#define KEY_MSG_GP2TRI   0x000caffe


int main(int argc, char* argv[]){
    srand(time(NULL));
    // inizializzazione di default delle variabili
    int numPazienti = DEFAULT_PAZIENTI,
    numReparti = DEFAULT_REPARTI,
    maxTempo = DEFAULT_TEMPO;
    // reinizializzazione delle variabili dal file di configurazione
    loadConfig(&numPazienti, &numReparti, &maxTempo);
    printf("Pazienti: %d\n", numPazienti);
    printf("Reparti: %d\n", numReparti);
    printf("Tempo: %d\n", maxTempo);


    //creazione e inizializzazione semaforo numero massimo pazienti
    int semIDnumPazienti = createSem(KEY_SEM_PAZIENTI, 1);
    initSem(semIDnumPazienti, 0, numPazienti);
    //creazione coda di messaggi da generatore pazienti verso triage
    int msgqIDgp2tri = createMsgQ(KEY_MSG_GP2TRI, true);


    //creo il triage
    pid_t pidTriage = fork();
    if (!pidTriage) {
        //triage(semIDnumPazienti, msgqIDgp2tri, numReparti);
        //chiamata a main triage
        exit(EXIT_SUCCESS);
    }


    //creo il generatore di pazienti
    pid_t pidGenPaz = fork();
    if (!pidGenPaz) {
        generaPazienti(msgqIDgp2tri);
        exit(EXIT_SUCCESS);
    }


    // aspetto maxTempo secondi e poi mando una SIGALARM per terminare tutto


    destroyMsgQ(msgqIDgp2tri);
    destroySem(semIDnumPazienti);
}

