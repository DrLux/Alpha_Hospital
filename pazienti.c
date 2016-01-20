#include "hospital.h"
#include "config.h"
#include "pazienti.h"

void pazienti(int msgqIDgp2tri){
	struct paziente persona;
	int totaleRighe = contaRighe("malattie.conf");

	loadMalattia((persona).malattia, getRand(1 , totaleRighe)); //numero random da 1 al totale delle righe (valore calcolato runtime)

}

//Pesca una malattia a caso dal file di testo e la inserisce nei dati del paziente
void loadMalattia(char *sintomo, int numRand){
    char* malattiaData;
    if (fileGetData("malattie.conf", &malattiaData)){
		int i;
		char* malattia = malattiaData;
		for (i=0; i < numRand; i++) {
			sintomo = strsep(&malattia, ",");
			printf("\nDebug sintomo %s Lunghezza sintomo %d \n", sintomo, (int)strlen(sintomo));//questo è per accertarmi che funzioni effettivamente. Il vero comando è quello che spara nella queue
			//sendMessage(msgqIDgp2tri, sintomo, (int)strlen(sintomo));
			strsep(&malattia, "\n");			
		}       
		free(malattiaData); 
    } else {
        printf("Problemi nella lettura di malattie.txt \n");
    }
}

//conta il numero di righe del file 
int contaRighe(char *path){
	int riga = 1;
	FILE *fp = fopen(path,"r");
	if (fp){
		char ch;
		while(!feof(fp)){
			ch = fgetc(fp);
			if(ch == '\n')
				riga++;
		}
	}
	fclose(fp);
	return riga;
}
