#include "hospital.h"
#include "config.h"
#include "comm.h"
#include "triage.h"
#include "pazienti.h"

#define DEFAULT_PAZIENTI 10
#define DEFAULT_REPARTI 2
#define DEFAULT_TEMPO 20

#define KEY_SEM_PAZIENTI 0xfaceb00c
#define KEY_MSG_GP2TRI   0x000caffe


#ifdef PRINT_COLOR
#define HOSPITAL_NAME ANSI_COLOR_BLUE "Hospital" ANSI_COLOR_RESET
#else
#define HOSPITAL_NAME "Hospital"
#endif

bool OSPEDALE_APERTO = true;
bool OSPEDALE_IN_CHIUSURA = false;


int main(int argc, char* argv[]){


    char* basePathConf = NULL;
    if (argc > 1) {
        if (!strcmp(argv[1], "help")){
            printf("\nUsage: %s [path]\n", argv[0]);
            printf("[path]: path cartella contentente le conf\n");
            printf("\t(Se nessun path e' specificato, le conf verranno cercate nella cartella attuale)\n\n");
            exit(EXIT_SUCCESS);
        } else {
            basePathConf = argv[1];
        }
    } 


    printf("\n["HOSPITAL_NAME"] AVVIATO\n");

    srand(getpid());
    //setbuf(stdout, NULL);

    // inizializzazione di default delle variabili
    int numPazienti = DEFAULT_PAZIENTI,
    numReparti = DEFAULT_REPARTI,
    maxTempo = DEFAULT_TEMPO;
    // reinizializzazione delle variabili dal file di configurazione
    loadConfig(&numPazienti, &numReparti, &maxTempo, basePathConf);
    printf("["HOSPITAL_NAME"] Pazienti: %d\n", numPazienti);
    printf("["HOSPITAL_NAME"] Reparti: %d\n", numReparti);
    printf("["HOSPITAL_NAME"] Tempo: %d\n", maxTempo);

    
    // creazione elenco "sintomi <--> reparto <--> gravita"
    struct elencoSintomi* sintomi = NULL;
    loadSintomi(&sintomi, basePathConf);


    //creazione e inizializzazione semaforo numero massimo pazienti
    int semIDnumPazienti = createSem(KEY_SEM_PAZIENTI, 1);
    initSem(semIDnumPazienti, 0, numPazienti);


    //creazione coda di messaggi da generatore pazienti verso triage
    int msgqIDPazienti2Triage = createMsgQ(KEY_MSG_GP2TRI, false);


    // settaggio handler SIGQUIT e SIGALRM (le "ereditano" tutti i figli)
    if (signal(SIGQUIT, sigquit_handler) == SIG_ERR) 
        printf("signal (SIGQUIT) error");
    if (signal(SIGALRM, sigalarm_handler) == SIG_ERR) 
        printf("signal (SIGALARM) error");



    //creo il triage
    pid_t pidTriage = fork();
    if (!pidTriage) {
        triage(semIDnumPazienti, msgqIDPazienti2Triage, numReparti, sintomi);
        //chiamata a main triage
        exit(EXIT_SUCCESS);
    }

    //creo il generatore di pazienti
    pid_t pidGenPaz = fork();
    if (!pidGenPaz) {
        generaPazienti(semIDnumPazienti, msgqIDPazienti2Triage, sintomi);
        exit(EXIT_SUCCESS);
    }


    // setto l'handler (solo per il padre) per propagare la SIGALRM ai figli
    if (signal(SIGALRM, sigalarm_handler_propagate) == SIG_ERR) 
        printf("signal (SIGALARM) error");

    
    //inizializzazione timer per la SIGALARM
    alarm(maxTempo);


    // ospedale operativo
    while(OSPEDALE_APERTO && !OSPEDALE_IN_CHIUSURA){
        sleep(1);
    }


    printf("["HOSPITAL_NAME"] ** ATTENDO FIGLI **\n");
    waitAllChild(); // aspetto che muoiano tutti i figli prima di liberare le risorse
    printf("["HOSPITAL_NAME"] ** CHIUDO **\n\n");


    // libero memoria elenco sintomi 
    int i;
    for (i=0; i<(*sintomi).numSintomi; i++) {
        free((*(*sintomi).arraySintomi[i]).sintomo);
        free((*sintomi).arraySintomi[i]);
    }
    free((*sintomi).arraySintomi);
    free(sintomi);

    // distruggo coda fra pazienti e triage 
    destroyMsgQ(msgqIDPazienti2Triage);
    // distruggo semaforo pazienti
    destroySem(semIDnumPazienti);
}


void sigquit_handler(int signum){
    if (signum == SIGQUIT){
        OSPEDALE_APERTO = false;
    }
}


void sigalarm_handler(int signum){
    if (signum == SIGALRM){
        OSPEDALE_IN_CHIUSURA = true;
    }
}

void sigalarm_handler_propagate(int signum){
    if (signum == SIGALRM){
        printf("["HOSPITAL_NAME"] tempo scaduto, chiudo l'ospedale\n");
        if (signal(SIGALRM, sigalarm_handler) == SIG_ERR) 
            printf("signal (SIGALARM) error");
        kill(-getpid(), SIGALRM);
    }
}


