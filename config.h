
/*

File header per gestire la lettura delle configurazioni da file

loadConfig    -> controlla se esiste il file di configurazione hospital.conf e richiama il parser per applicare le conf
parseConf     -> parsifica il contenuto delle configurazioni, cercando su ogni riga il nome della varibaile da inizializzare e il valore da assegnargli
fileLen       -> ottiene la lunghezza in byte del file
fileGetData   -> crea una malloc e ritorna un puntatore a char contenente una copia del contenuto del file terminato da un byte nullo
isInt         -> verifica che la stringa passata sia un numero, anche preceduto da + o -
getNextLexeme -> salta tutti gli spazi e i tab finche trova una stringa, quando la trova ne ritorna l'indirizzo

*/


void loadConfig(int* numPazienti, int* numReparti, int* maxTime);
void parseConfig(char* data, int* numPazienti, int* numReparti, int* maxTempo);
bool verifyRangeConfig(char* name, int num, int min, int max);
char* getNextLexeme(char** string);
size_t fileLen(FILE *fp);
bool fileGetData(char* fileName, char** data);
bool isInt(char* c);


