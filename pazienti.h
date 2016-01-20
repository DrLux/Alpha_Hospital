
void generaPazienti(int semPazienti, int msgqIDgp2tri, struct elencoSintomi* sintomi);
int countLine(char *path);
int getRand(int min, int max);
int lunghezzaStringa(char *sintomo);
int randomNumber();

void loadSintomi(struct elencoSintomi** sintomi);
void parseSintomi(char* data, struct elencoSintomi** sintomi);
char* getSintomoRantom(struct elencoSintomi* sintomi);
