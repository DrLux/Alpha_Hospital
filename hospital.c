#include "hospital.h"
#include "config.h"
#include "comm.h"
#include "triage.h"
#include "pazienti.h"
//#include "cartellaPaziente.h"





#define DEFAULT_PAZIENTI 10
#define DEFAULT_REPARTI 2
#define DEFAULT_TEMPO 20

#define KEY_SEM_PAZIENTI 0xfaceb00c
#define KEY_MSG_GP2TRI  0x13371337





int main(int argc, char* argv[]){

    srand(getpid());
    // inizializzazione di default delle variabili
    int numPazienti = DEFAULT_PAZIENTI,
    numReparti = DEFAULT_REPARTI,
    maxTempo = DEFAULT_TEMPO;
    // reinizializzazione delle variabili dal file di configurazione
    loadConfig(&numPazienti, &numReparti, &maxTempo);
    printf("Pazienti: %d\n", numPazienti);
    printf("Reparti: %d\n", numReparti);
    printf("Tempo: %d\n", maxTempo);




    
    struct elencoSintomi* sintomi = NULL;
    loadSintomi(&sintomi);

    //printf("Sintomi: %d\n", (*sintomi).numSintomi);

    //int i;
    //for (i=0; i<(*sintomi).numSintomi; i++) {
    //    printf("[Parsed] %s, %d, %d\n", (*(*sintomi).arraySintomi[i]).sintomo, (*(*sintomi).arraySintomi[i]).reparto, (*(*sintomi).arraySintomi[i]).gravita);
    //}

    /*
    int a;
    for (a=0; a<5; a++) {
        //printf("[%d] %s, %d, %d\n", a, malattie[a]->sintomo, malattie[a]->reparto, malattie[a]->gravita);
        printf("[%d] %s, %d, %d\n", 4, (*malattie[a]).sintomo, (*malattie[a]).reparto, (*malattie[a]).gravita);
    }

    for (a=0; a<5; a++) {
        free((*malattie[a]).sintomo);
        free(malattie[a]);
    }
    free(malattie);
    */
    //loadMalattie();
    /*END TEST */


    //creazione e inizializzazione semaforo numero massimo pazienti
    int semIDnumPazienti = createSem(KEY_SEM_PAZIENTI, 1);
    initSem(semIDnumPazienti, 0, numPazienti);
    //creazione coda di messaggi da generatore pazienti verso triage
    int msgqIDgp2tri = createMsgQ(KEY_MSG_GP2TRI, true);


    //creo il triage
    pid_t pidTriage = fork();
    if (!pidTriage) {
        triage(semIDnumPazienti, msgqIDgp2tri, numReparti, sintomi);
        //chiamata a main triage
        exit(EXIT_SUCCESS);
    }


    //creo il generatore di pazienti
    pid_t pidGenPaz = fork();
    if (!pidGenPaz) {
        generaPazienti(msgqIDgp2tri, sintomi);
        exit(EXIT_SUCCESS);
    }


    // aspetto maxTempo secondi e poi mando una SIGALARM per terminare tutto


    // libero memoria sintomi (forse da spostare se questo main termina prima di triage)
    int i;
    for (i=0; i<(*sintomi).numSintomi; i++) {
        free((*(*sintomi).arraySintomi[i]).sintomo);
        free((*sintomi).arraySintomi[i]);
    }
    free((*sintomi).arraySintomi);
    free(sintomi);

    destroyMsgQ(msgqIDgp2tri);
    destroySem(semIDnumPazienti);
}






