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
#define KEY_MSG_GP2TRI   0x000caffe





int main(int argc, char* argv[]){

    srand(getpid());
    //srand(time(NULL));
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
    int msgqIDgp2tri = createMsgQ(KEY_MSG_GP2TRI, false);
    //struct msqid_ds queue_ds;
    //getInfo(msgqIDgp2tri, &queue_ds);


    /*
    //creazione Handler per la SIGALARM
    if (signal(SIGALRM, chiusuraOspedale) == SIG_ERR) //sigalarm = 14
        printf("signal (SIG_ERR) error");
    //inizializzazione timer per la SIGALARM
    alarm(maxTempo);
    */

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
        generaPazienti(semIDnumPazienti, msgqIDgp2tri, sintomi);
        exit(EXIT_SUCCESS);
    }


    // aspetto maxTempo secondi e poi mando una SIGALARM per terminare tutto

    sleep(maxTempo);

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

/*
//CHIEDERE DOVE METTERE QUESTO PROTOTIPO
static void chiusuraOspedale(int sig){
    if (sig == SIGALRM)
      GLOBAL_SWITCH = 1;
    if (sig == SIGQUIT)
      GLOBAL_SWITCH = 2;
    return;
}
*/
