/*

File header per gestire:
- semafori
- code di messaggi
- altro

createSem    -> genera "num" semafori aventi la stessa key passata come parametro
destroySem   -> distrugge il semaforo 
initSem      -> inizializza il semaforo indicato dai parametri al valore passato come terzo parametro
semReserve   -> decrementa il semaforo
semRelease   -> incrementa il semaforo
semGetVal    -> ottiene il valore del semaforo

createMsgQ   -> Crea una coda di messaggi con la key passata come parametro
destroyMsgQ  -> distrugge la coda di messaggi
sendMessage  -> Scrivo messaggi in coda
recvMessage  -> Ottengo messaggi in base al tipo

waitAllChild -> aspetta che muoiano tutti i figli di un processo
getRand      -> genera un numero random compreso tra min_num e max_num (compresi)

*/


// su mac non serve
#if defined(__linux__)
union semun {
        int val;
        struct semid_ds* buf;
        unsigned short* array;
        #if defined(__linux__)
        struct seminfo* __buf;
        #endif
};
#endif


int createSem(int key, int num);
void destroySem(int semid);
void initSem(int semid, int semnum, int val);
void semReserve(int semid, int semnum);
void semRelease(int semid, int semnum);
int semGetVal(int semid, int semnum);

int createMsgQ(int key, bool attachIfExists);
void destroyMsgQ(int msgqid);
void sendMessage(int msgid, void *msg, int msgSize);
bool recvMessage(int msgid, void *msg, int msgSize, long msgtype);

void waitAllChild();
int getRand(int min_num, int max_num);

