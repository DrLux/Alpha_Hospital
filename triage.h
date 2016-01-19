void triage(int semPazienti, int msgqPazienti, int reparti);
bool createChilds(int n, int* pids);


void triage(int semPazienti, int msgqPazienti, int reparti){

	int* pidsReparti = (int*) malloc(reparti*sizeof(int));
	bool isChild = createChilds(reparti, pidsReparti);

	if (isChild) { // routine reparti
		// routine principale reparto
		exit(EXIT_SUCCESS);
	}

	int i;
	for (i=0; i<reparti; i++) {
		printf("[%d] %d\n", i, pidsReparti[i]);
	}


	bool accept = false; // TRUE
	while(accept){
		semReserve(semPazienti, 0); // decremento il semaforo dei pazienti

	}

	free(pidsReparti);
}

bool createChilds(int n, int* pids){
	bool isChild = false;
	int i;
	for (i=0; i<n && !isChild; i++) {
		int pid = fork();
		if(!pid)
			isChild = true;
		else
			pids[i] = pid;
	}
	return isChild;
}