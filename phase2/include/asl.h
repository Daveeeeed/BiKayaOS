#ifndef ASL_H
#define ASL_H
#include "types_bikaya.h"

// Ritorna il semaforo con chiave pari a key, se
// esiste, altrimenti NULL
semd_t* getSemd(int *key);

// Inizializza pcb_free in modo da contenere tutti
// gli elementi della semd_table
void initASL();

// Viene inserito il PCB puntato da p nella coda dei
// processi bloccati del SEMD con chiave pari a key.
// Se il semaforo corrispondente non è presente nella
// lista ne alloca uno nuovo dal semd_free, inserendolo
// in semd_busy. Se semd_free è vuota e non contiene
// elementi da allocare ritorna TRUE, in tutti gli altri
// casi FALSE
int insertBlocked(int *key, struct pcb_t *p);

// Ritorna e rimuove dalla coda dei processi
// del semaforo che ha come chiave key il primo
// elemento. Se dopo la rimozione la lista diventa
// vuota, rimuove il SEMD rendendolo nuovamente un 
// semaforo libero e inutilizzato
pcb_t* removeBlocked(int *key);

// Rimuove il PCB puntato da p dalla coda del semaforo
// su cui è bloccato. Se il PCB non compare in tale coda,
// allora restituisce NULL, altrimenti, restituisce p.
// Se la coda dei processi bloccati per il semaforo diventa vuota,
// rimuove il descrittore corrispondente dalla semd_busy e lo
// inserisce nella coda dei descrittori liberi (semd_free).
pcb_t* outBlocked(pcb_t *p);

// Restituisce il puntatore al PCB in testa alla coda dei processi
// associata al SEMD con chiave key. Ritorna NULL se il SEMD non
// compare nella semd_busy oppure se compare ma la sua coda dei
// processi è vuota
pcb_t* headBlocked(int *key);

// Rimuove il PCB puntato da p dalla coda del semaforo
// su cui è bloccato. Inoltre, elimina tutti i processi
// dell’albero radicato in p dalle eventuali code dei
// semafori su cui sono bloccati.
void outChildBlocked(pcb_t *p);

struct list_head getSemdFree();

#endif