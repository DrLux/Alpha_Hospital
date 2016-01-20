#include "reparto.h"

void triage(int semPazienti, int msgqPazienti, int reparti, struct elencoSintomi* sintomi);
bool createChildsWithQueue(int n, int* msgqids, int* repmsgqid);
void getRepartoGravita(struct elencoSintomi* sintomi, char* sintomoRicevuto, int* reparto, int* gravita);
