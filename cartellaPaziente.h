void malattiaPaziente(struct paziente *msg);
void loadMalattia(struct paziente *msg , int numRand);
int contaRighe(char *path);
int getRand(int min , int max);
int randomNumber();

//riempie i dati del paziente
void malattiaPaziente(struct paziente *msg){
	int length_malattia = 10; //ESEMPIO
	(*msg).reparto = 0;
	(*msg).mtype = 0;
	(*msg).malattia =(char *) malloc(sizeof(char) * length_malattia);
	int totaleRighe = contaRighe("malattie.txt");
	if ( totaleRighe == 0){
		printf(" Errore nella lettura del file in malattiaPaziente ");
				exit(EXIT_FAILURE);
	}
	loadMalattia(msg, getRand(1 , totaleRighe)); //numero random da 1 al totale delle righe (valore calcolato runtime)
}

//Pesca una malattia a caso dal file di testo e la inserisce nei dati del paziente
void loadMalattia(struct paziente *msg , int numRand){
    char* malattiaData;
	int i = 0;
    if (fileGetData("malattie.txt", &malattiaData)){
		for (i ; i < numRand ; i++){
			(*msg).malattia = strsep(&malattiaData, ",");
			strsep(&malattiaData, "\n");
		}       
	//free(malattiaData); //NON FUNZIONA LA FREEE OCCHIO!!! da sistemare
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

//genera un numero randomico entro un certo range di valori
int getRand(int min , int max){
	srand(time(NULL)+getpid());
	int rand = 0;
	while ( rand < min || rand > max)
		rand = randomNumber();
	return rand;
}

//genera un numero randomicamente
int randomNumber(){
  return rand()%10;
}