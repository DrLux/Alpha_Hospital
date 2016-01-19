#include "hospital.h"
#include "config.h"

void loadConfig(int* numPazienti, int* numReparti, int* maxTempo){
    char* confData;
    if ( fileGetData("hospital.conf", &confData) ){
        parseConfig(confData, numPazienti, numReparti, maxTempo);
        free(confData);
    } else {
        printf("hospital.conf non trovato! Applico le configurazioni di default\n");
    }
}


void parseConfig(char* data, int* numPazienti, int* numReparti, int* maxTempo){
    char* line;
    while ( (line = strsep(&data, "\n")) ) { // per ogni riga del file...
        if (*line) {
            char* paramName = getNextLexeme(&line); // ottengo il nome del parametro

            // controllo che non ci siano puntatori a null
            if (paramName && *paramName && line && *line) {
                char* paramVal = getNextLexeme(&line); // ottengo il valore del parametro
                if (!strcmp(paramName, "pazienti")) {

                    if (isInt(paramVal))
                        *numPazienti = atoi(paramVal); // assegno il valore alla variabile

                } else if (!strcmp(paramName, "reparti")){

                    if (isInt(paramVal))
                        *numReparti = atoi(paramVal);

                } else if (!strcmp(paramName, "tempo")){

                    if (isInt(paramVal))
                        *maxTempo = atoi(paramVal);

                }
            }
        }
    }
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


// ottiene la lunghezza del file in byte
size_t fileLen(FILE *fp){
    size_t fileSize = -1;
    if (fp){
        fseek(fp, 0, SEEK_END);
        fileSize = ftell(fp);
        fseek(fp, 0, SEEK_SET);
    }
    return fileSize;
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


bool isInt(char* c){
    if (*c == '-' || *c == '+')
        c++;
    do {
        if (!isdigit(*c++))
            return false;
    } while (*c);
    return true;
}

