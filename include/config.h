
/*

File header per gestire la lettura delle configurazioni da file

loadConfig        -> controlla se esiste il file di configurazione hospital.conf e richiama il parser per applicare le conf
parseConf         -> parsifica il contenuto delle configurazioni, cercando su ogni riga il nome della varibaile da inizializzare e il valore da assegnargli
fileGetData       -> crea una malloc e ritorna un puntatore a char contenente una copia del contenuto del file terminato da un byte nullo
getNextLexeme     -> salta tutti gli spazi e i tab finche trova una stringa, quando la trova ne ritorna l'indirizzo
verifyRangeConfig -> verifica che il numero sia compreso tra due valori
fileLen           -> ottiene la lunghezza in byte del file
isInt             -> verifica che la stringa passata sia un numero, anche preceduto da + o -


*/


void loadConfig(int* numPazienti, int* numReparti, int* maxTempo, char* basePathConf);
char* makePath(char* basePath, char* fileName);
void parseConfig(char* data, int* numPazienti, int* numReparti, int* maxTempo);
bool fileGetData(char* fileName, char** data);
char* getNextLexeme(char** string);
bool verifyRangeConfig(char* name, int num, int min, int max);
size_t fileLen(FILE *fp);
bool isInt(char* c);

#define DEFAULT_CONF_DIR "."
#define HOSPITAL_CONF "hospital.conf"
#define SINTOMI_CONF "sintomi.conf"


