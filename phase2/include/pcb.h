#ifndef _PCB_H
#define _PCB_H
#include "types_bikaya.h"

// Inizializza pcb_free in modo da contenere tutti
// gli elementi della pcb_table
void initPcbs(void);

// Inserisce il processo puntato da p in pcb_free
void freePcb(pcb_t *p);

// Se pcb_free non è vuota, ne rimuove un elemento
// inizializzandone i valori (0/NULL) e ritornando
// un puntatore ad esso, NULL altrimenti
pcb_t *allocPcb(void);

// Inizializza la lista dei PCB, inizializzandone
// la sentinella head passata come parametro
void mkEmptyProcQ(struct list_head *head);

// Verifica il riempimento della lista tramite
// la sentinella head passata come parametro:
// TRUE se la lista è vuota, FALSE altrimenti
int emptyProcQ(struct list_head *head);

// Inseriesce l'elemento puntato da p nella lista dei
// processi che ha come sentinella head, rispettandone
// la priorità (pcb_t.priority)
void insertProcQ(struct list_head *head, struct pcb_t *p);

// Ritorna l'elemento in testa alla lista dei processi
// che ha come sentinella head, senza eliminarlo.
pcb_t *headProcQ(struct list_head *head);

// Rimuove e ritorna il primo elemento in testa alla lista
// dei processi con sentinella head, NULL se la lista è vuota
pcb_t *removeProcQ(struct list_head *head);

// Rimuove e ritorna il PCB puntato da p dalla lista dei
// processi con sentinella head. Se il PCB è contenuto
// nella lista ne ritorna un puntatore, NULL altrimenti
pcb_t *outProcQ(struct list_head *head, pcb_t *p);

// Verifica la presenza di figli del PCB puntato da this
// TRUE, se non ha figli, FALSE altrimenti
int emptyChild(pcb_t *this);

// Inserisce il PCB puntato da p come figlio del PCB
// puntato da prnt
void insertChild(pcb_t *prnt, pcb_t *p);

// Rimuove il PCB puntato da p dalla lista dei figli del padre.
// Se tale elemento non ha un padre viene restituito NULL, altrimenti
// un puntatore all'elemento p
struct pcb_t *outChild(struct pcb_t *p);

// Rimuove il primo figlio del PCB puntato da p.
// Se esiste ne ritorna un puntatore, NULL altrimenti
pcb_t* removeChild(pcb_t *p);

#endif