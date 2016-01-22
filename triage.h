#include "reparto.h"

void triage(int semPazienti, int msgqPazienti, int reparti, struct elencoSintomi* sintomi);
bool createChildsWithFifo(int n, int* fifoids, char* repfifopath, int* repid);
void removeFifos(int n, int* fifoids, char* repfifopath);
void getRepartoGravita(struct elencoSintomi* sintomi, char* sintomoRicevuto, int* reparto, int* gravita);
