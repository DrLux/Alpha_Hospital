/*

File header per caricare i sintomi da file e generare clienti verso il triage 

generaPazienti   -> abbassa il semaforo dei pazienti, genera un sintomo random e lo invia a triage
loadSintomi      -> legge il file sintomi.conf e lo passa a parseSintomi
parseSintomi     -> crea un array di puntatori a strutture schedaSintomo, popolate leggendo il file sintomi.conf
getSintomoRandom -> genera un numero random e ritorna il puntatore a un sintomo
countLine        -> ritorna il numero di linee di un file
rangeRestrict    -> se num e' maggiore di max, torna max; se num e' minore di min, torna min; altrimeni torna num

*/


void generaPazienti(int semIDPazienti, int msgqIDTriage, struct elencoSintomi* sintomi);
void loadSintomi(struct elencoSintomi** sintomi);
void parseSintomi(char* data, struct elencoSintomi** sintomi);
char* getSintomoRandom(struct elencoSintomi* sintomi);
int countLine(char *path);
int rangeRestrict(int num, int min, int max);

