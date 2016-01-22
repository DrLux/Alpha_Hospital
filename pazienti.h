
void generaPazienti(int semPazienti, int msgqIDgp2tri, struct elencoSintomi* sintomi);
void loadSintomi(struct elencoSintomi** sintomi);
void parseSintomi(char* data, struct elencoSintomi** sintomi);
int countLine(char *path);
int rangeRestrict(int num, int min, int max);
char* getSintomoRandom(struct elencoSintomi* sintomi);


