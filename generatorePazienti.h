//struttura che definisce il paziente
struct paziente {
 char *malattia; /* Nome della malattia */
 int mtype; /* GRAVITA Indice (da 1 a 10) di gravita della malattia del paziente */
 int reparto; /* Reparto associato alla specifica malattia */
}; 

int createCodeMessage (int key, int permessi);
void getMessage(int msgid, struct paziente *msg, long msgtype);
void setMessage(int msgid, struct paziente *msg, long msgtype);

void GeneratorePazienti(int totPazienti , int tempo , int key , int permessi);
void malattiaPaziente(struct paziente *msg );

// Crea una coda di messaggi con la key passata come parametro
int createCodeMessage(int key , int permessi){
	int msgid; 
	if ((msgid = msgget( key, permessi | IPC_CREAT | IPC_EXCL)) < 0){
		printf("Error msgget\n");
		exit(EXIT_FAILURE);
	}
	return msgid;
}

// Scrivo messaggi in coda
void setMessage(int msgid, struct paziente *msg, long msgtype){
	if( msgsnd(msgid, &msg, sizeof(msg), msgtype) == -1){	
		printf("\nmsgsnd error");	 
		exit(EXIT_FAILURE);
	} 
}
	
// Ottengo messaggi in base al tipo -------------> DA FINIRE
void getMessage(int msgid, struct paziente *msg, long msgtype){
        if (msgrcv(msgid, msg, sizeof(*msg), msgtype, IPC_NOWAIT) == -1) { 
                printf("None message with type %ld\n", msgtype);
				exit(EXIT_FAILURE);
        } else {
                //(*msg).mtext[63] = '\0';
                //printf("Contenuto: \"%s\"\n", (*msg).mtext);
                //fflush(stdout);
        }
}

void GeneratorePazienti(int totPazienti , int tempo , int key , int permessi){
	int childPid = 1;
	int msgid = createCodeMessage(key , permessi);
	struct paziente msg;
	for (totPazienti ; totPazienti > 0 && childPid != 0 ; totPazienti--) // vogliamo che solo il padre continui a ciclare
		childPid = fork(); // il childPid dei figli verrà settato a 0 e usciranno dal ciclo
		
	switch (childPid) {
		case -1:{
			printf("Errore fork del generatore di pazienti");
		break;
		}		
		
		case 0:{
			malattiaPaziente(&msg); //riempie la struttura dati paziente
			printf("\nSono il figlio numero %d ed il mio Messaggio e': %s \n", getpid() , msg.malattia);
			setMessage(msgid, &msg , 0);
			exit(0);
		break;
		}
		
		default:{
			sleep(tempo);
			printf("\nSono il padre e il mio id e' %d" , getpid());
			printf("\nTempo scaduto!"); // questo è un controllo fittizio di test PROVVISORIO, in realta il tempo si gestisce con un handler
		break;
		}
	}	
	
	//test 
		while (wait(NULL) != -1)
			continue;
		if (errno != ECHILD) // errore vero
				printf("Errore wait"); 													
				
		printf("\nOra che tutti i figli sono morti continuo a vivere la mia vita in santa pace, sono il padre\n");
		/*if (msgctl(msgid, IPC_RMID, &msg) == -1) {
                printf("\nFailure in msgctl(IPC_RMID)\n");
				exit(EXIT_FAILURE);
        } else {
                printf("Coda di pazienti eliminata\n");
        }*/
}