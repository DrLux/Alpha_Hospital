
//struttura che definisce il paziente
struct paziente {
	char *malattia; /* Nome della malattia */
	int mtype; /* GRAVITA Indice (da 1 a 10) di gravita della malattia del paziente */
	int reparto; /* Reparto associato alla specifica malattia */
}; 


void GeneratorePazienti(int totPazienti, int tempo, int key, int permessi);
void malattiaPaziente(struct paziente *msg);







void generaPazienti(int totPazienti, int tempo, int key, int permessi){
	int childPid = 1;
	int msgQueueID = createMsgQueue(key, permessi);
	struct paziente msg;

	for (totPazienti; totPazienti > 0 && childPid != 0; totPazienti--) // vogliamo che solo il padre continui a ciclare
		childPid = fork(); // il childPid dei figli verrà settato a 0 e usciranno dal ciclo
		
	switch (childPid) {
		case -1:{
			printf("Errore fork del generatore di pazienti");
			break;
		}		
		
		case 0:{
			malattiaPaziente(&msg); //riempie la struttura dati paziente
			printf("\nSono il figlio numero %d ed il mio Messaggio e': %s \n", getpid() , msg.malattia);
			setMessage(msgid, &msg, 0);
			exit(0);
			break;
		}
		
		default:{
			sleep(tempo); // questo è un controllo fittizio di test PROVVISORIO, in realta il tempo si gestisce con un handler
			getMessage(msgid, &msg, 0);
			printf("\nTempo scaduto!");
			printf("\nSono il padre e il mio id e' %d\n" , getpid());
			break;
		}
	}	

	// routine di test 
	while (wait(NULL) != -1)
		continue;
	if (errno != ECHILD) // errore vero
			printf("Errore wait"); 															

	
}


