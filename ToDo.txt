ToDo.txt

1) accedere ad una malloca da un altro thread (ci serve per reparto malattia in triage ed in generatore pazienti) --| il figlio mantiene nella sua memoria una copia di una malloc del padre

2) modificare una variabile all' interno del main tramite un handler di signal --> no, ma si puo modificare una globale attraverso l' handler. Poi nel while si mette un if per vedere se la globale è stata modificata e in quel caso decidere se fermarsi o meno.

3) la rcv message è bloccante o no? SI, normalmente se non ci sono messaggi aspetta finche arrivano. Volendo si puo non rimanere in attesa con IPC_NOWAIT e far tornare un errore ENOMSG

4) una signal viene ereditata automaticamente dai figli o no? --> No il signal viene bloccato dal primo che lo intercetta. Pero egli stesso nell' handler puo rimbalzare il 
segnale agli altri handler creando un interrupt virtuale



- se il paziente viene lasciato morire per mancanza di reparto ricordarsi di modificare il semaforo
- il triage prende la priorita è invia la gravita in gruppo ciclico, es: gravita 8 -> priorita 2. Prestazioni eseguite da 0 a 10