/*  Copyright (C) 2015  Carlo Stomeo, Stefano Mazza, Alessandro Zini, Mattia Maldini
    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.
    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.
    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.*/


#include "listx.h"
#include "const.h"
#include "pcb.h"
#include "asl.h"

#define HIDDEN static


/*Modulo contenente le funzione di gestione delle liste di semafori*/

//lista di semafori liberi
HIDDEN LIST_HEAD(semd_free);

//lista di semafori attivi
HIDDEN LIST_HEAD(semd_busy);

//descrittori di semafori disponibili
HIDDEN struct semd_t semd_table[MAXPROC];

//rimuove il semaforo q dalla lista di quelli attivi se non ha processi bloccati
HIDDEN inline void rem_idle_sem( struct semd_t* q){
	if( list_empty( &(q->s_procQ) )){
		list_del( &(q->s_next) );
		list_add( &(q->s_next), &semd_free );
	}
}

// Ritorna il SEMD contenente target
HIDDEN semd_t* semdOf(struct list_head *target){
    return container_of(target, semd_t, s_next);
}

// Ritorna la chiave associata al SEMD contenente target
HIDDEN int* keyOf(struct list_head *target){
    return semdOf(target)->s_key;
}

// Restituisce il puntatore al SEMD nella semd_busy la cui
// chiave è pari a key. Se non esiste un elemento nella
// semd_busy con chiave uguale a key, viene restituito NULL.
semd_t* getSemd(int *key){
    struct list_head *tmp;
    tmp = semd_busy.next;
    semd_t *target;
    while (tmp != &semd_busy){
        target = semdOf(tmp);
        if (target->s_key == key) return target;
        tmp = tmp->next;
    }
    return NULL;
}


//inizializza la lista di semafori liberi
void initASL(){
    int i;
    for( i = 0; i < MAXPROC; i++ ){
    INIT_LIST_HEAD( &semd_table[i].s_procQ ) ;
        list_add( &(semd_table[i].s_next) , &(semd_free) );
    }
}
 
int insertBlocked(int *key, struct pcb_t *p){
    if(p == NULL || key == NULL) return ERROR;
    struct list_head *tmp_free, *tmp_busy; 
    struct semd_t* target;
    target = getSemd(key);
    if(target == NULL){
        if(list_empty(&(semd_free))) return TRUE;
        tmp_busy = semd_busy.next;
        while(tmp_busy != &semd_busy && (keyOf(tmp_busy) < key)) tmp_busy = tmp_busy->next;
        tmp_free = semd_free.next;
        list_del(tmp_free);
        target = container_of( tmp_free , struct semd_t , s_next );
        target->s_key = key;
        __list_add(tmp_free, tmp_busy->prev, tmp_busy);
    }
    p->p_semkey = key;
    list_add_tail(&(p->p_next), &(target->s_procQ));
    return FALSE;
}

//Rimuove il primo processo dalla lista dei processi bloccati del semaforo puntato da semAdd  
struct pcb_t *removeBlocked(int *semAdd){
    struct semd_t* q;
    struct pcb_t* p ;
    q = getSemd( semAdd );
    if( q != NULL ){
	p = removeProcQ( &(q->s_procQ) );
        rem_idle_sem(q);
	return p ;	
    }
    else return NULL;   /*return NULL se nessun semAdd non viene trovato*/
}


//Rimuove il processo p dalla lista di processi bloccati del semaforo su cui è bloccato
//(se bloccato su un semaforo)
struct pcb_t *outBlocked(struct pcb_t *p){
    if( p == NULL ) return NULL;
    struct semd_t *sem ;
    if( p->p_semkey == NULL ) return NULL;
    sem = getSemd( p->p_semkey );
    if( sem == NULL ) return NULL ;
    p = outProcQ(  &(sem->s_procQ) , p ); 
    if(p!= NULL) rem_idle_sem( getSemd(p->p_semkey) );
    return p;
}

void outChildBlocked(struct pcb_t *p) {
    struct list_head *aux;

    if (p == NULL)
        return;

    aux = p->p_child.next;
    while (aux != &(p->p_child) && aux != NULL) {
        outChildBlocked(container_of(aux, struct pcb_t, p_sib));
        outBlocked(container_of(aux, struct pcb_t, p_sib));
        aux = aux->next;
    }
    outBlocked(p);
}
 

//Restituisce il primo processo bloccato sul semaforo puntato da semAdd senza rimuoverlo
struct pcb_t *headBlocked(int *semAdd){
    struct semd_t* q;
    q = getSemd( semAdd );
    if( q == NULL ) return NULL;
    return headProcQ( &q->s_procQ );
}