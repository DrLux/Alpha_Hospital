#include "hospital.h"
#include "config.h"

// controlla se esiste il file di configurazione hospital.conf e richiama il parser per applicare le conf
void loadConfig(int* numPazienti, int* numReparti, int* maxTempo, char* basePathConf){
    char* confPath;
    if (basePathConf!=NULL)
        confPath = makePath(basePathConf, HOSPITAL_CONF);
    else
        confPath = makePath(DEFAULT_CONF_DIR, HOSPITAL_CONF);

    char* confData;
    if ( fileGetData(confPath, &confData) ){
        parseConfig(confData, numPazienti, numReparti, maxTempo);
        free(confData);
    } else {
        puts("[ERRORE] hospital.conf non trovato! Applico le configurazioni di default");
    }
    free(confPath);
}


// parsifica il contenuto delle configurazioni, cercando su ogni riga il nome della varibaile da inizializzare e il valore da assegnargli
void parseConfig(char* data, int* numPazienti, int* numReparti, int* maxTempo){
    char* line;
    while ( (line = strsep(&data, "\n")) ) { // per ogni riga del file...
        if (*line) {
            char* paramName = getNextLexeme(&line); // ottengo il nome del parametro

            // controllo che non ci siano puntatori a null
            if (paramName && *paramName && line && *line) {
                char* paramVal = getNextLexeme(&line); // ottengo il valore del parametro
                if (!strcmp(paramName, "pazienti")) {

                    if (isInt(paramVal) && verifyRangeConfig(paramName, atoi(paramVal), 1, INT_MAX))
                        *numPazienti = atoi(paramVal); // assegno il valore alla variabile

                } else if (!strcmp(paramName, "reparti")){

                    if (isInt(paramVal) && verifyRangeConfig(paramName, atoi(paramVal), 1, 10))
                        *numReparti = atoi(paramVal);

                } else if (!strcmp(paramName, "tempo")){

                    if (isInt(paramVal) && verifyRangeConfig(paramName, atoi(paramVal), 1, INT_MAX))
                        *maxTempo = atoi(paramVal);

                }

            }
        }
    }
}

// crea una malloc con il contenuto del file terminato da NULL
bool fileGetData(char* fileName, char** data){
    bool success = false;
    FILE* fp = fopen(fileName, "r");
    if (fp) {
        size_t fileSize = fileLen(fp);
        *data = (char*) malloc(sizeof(char)*fileSize+1); // +1 per terminazine stringa
        fread(*data, fileSize, 1, fp);
        fclose(fp);
        (*data)[fileSize] = '\0'; // aggiungo terminatore di stringa
        success = true;
    }
    return success;
}


// ottiene una sottostringa delimitata da spazi o tab
char* getNextLexeme(char** string) {
    char* lexeme;
    while ( (lexeme = strsep(string, " \t")) ) {
        //termino quando ho trovato l'inizio di una sottostringa oppure ho finito la stringa
        if (*lexeme != '\0' || **string == '\0')
            break;
    }
    return lexeme;
}

// concatena due stringhe separate da / in una nuova stringa e ne ritorna il puntatore
char* makePath(char* basePath, char* fileName){
    char* path = (char*) malloc( ( strlen(basePath) + 1 + strlen(fileName) + 1 ) * sizeof(char) ); // conf/file.conf\0
    sprintf(path, "%s/%s", basePath, fileName);
    return path;
}


// verifica che il numero sia compreso tra due valori
bool verifyRangeConfig(char* name, int num, int min, int max) {
    if (num >= min && num <= max)
        return true;
    else {
        printf("Valore '%s' invalido, applico default\n", name);
        return false;
    }
}


// ottiene la lunghezza del file in byte
size_t fileLen(FILE *fp){
    size_t fileSize = -1;
    if (fp){
        fseek(fp, 0, SEEK_END); // si sposta alla fine del file
        fileSize = ftell(fp); // ritorna la distanza dall'inizio
        fseek(fp, 0, SEEK_SET); // torna all'inizio
    }
    return fileSize;
}


// verifica che una stringa sia la rappresentazione di un intero
bool isInt(char* c){
    if (*c == '-' || *c == '+') // accetta numeri preceduti da + o -
        c++;
    do {
        if (!isdigit(*c++))
            return false;
    } while (*c);
    return true;
}


