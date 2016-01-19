void generaPazienti();
void loadMalattia(char *sintomo , int numRand);
int contaRighe(char *path);
int randomNumber();
int getRand(int min , int max);

void generaPazienti(int msgqIDgp2tri , int semIDnumPazienti){
	char* sintomo;
	int totaleRighe = contaRighe("malattie.conf");
	loadMalattia(sintomo, getRand(1 , totaleRighe)); //numero random da 1 al totale delle righe (valore calcolato runtime)
	//sendSintomo(msgqIDgp2tri, &sintomo, 0);
}

//Pesca una malattia a caso dal file di testo e la inserisce nei dati del paziente
void loadMalattia(char *sintomo , int numRand){
    char* malattiaData;
    if (fileGetData("malattie.conf", &malattiaData)){
		char* malattia = malattiaData; //Variabile d' appoggio per poter fare la free, altrimenti veniva fatta su malattiadata puntatore che non puntava all'inizio della malloc
		int i;
		for (i=0; i < numRand; i++) {
			sintomo = strsep(&malattia, ",");
			printf("\n Debug %s\n", sintomo);
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

//genera un numero randomicamente
int randomNumber(){
	return rand()%10;
}

//genera un numero randomico entro un certo range di valori
int getRand(int min , int max){
	srand(time(NULL)+getpid());
	int rand = 0;
	while ( rand < min || rand > max)
		rand = randomNumber();
	return rand;
}