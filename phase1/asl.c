#include "asl.h"
#include "const.h"
#include "pcb.h"


#define HIDDEN static

// Array di SEMD disponibili
HIDDEN semd_t semd_table[MAXPROC];

// Lista dei SEMD liberi o inutilizzati
HIDDEN LIST_HEAD(semd_free);

// Lista dei SEMD attivi
HIDDEN LIST_HEAD(semd_busy);

// Ritorna il SEMD contenente target
HIDDEN semd_t* semdOf(struct list_head *target){
    return container_of(target, semd_t, s_next);
}

HIDDEN int* keyOf(struct list_head *target){
    return semdOf(target)->s_key;
}

semd_t* getSemd(int *key){
    struct list_head *tmp = semd_busy.next;
    semd_t *target;
    while (tmp != &semd_busy){
        target = semdOf(tmp);
        if (target->s_key == key) return target;
        tmp = tmp->next;
    }
    return NULL;
}

void initASL(){
    for (int i = 0; i < MAXPROC; i++){
        semd_table[i].s_key = NULL;
        semd_table[i].s_next.next = NULL;
        semd_table[i].s_next.prev = NULL;
        INIT_LIST_HEAD( &semd_table[i].s_procQ );
        list_add_tail(&semd_table[i].s_next, &semd_free);
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

pcb_t* removeBlocked(int *key){
    struct pcb_t* pcb_target;
    struct semd_t* semd_target;
    semd_target = getSemd(key);
    if (semd_target != NULL){
        pcb_target = removeProcQ(&(semd_target->s_procQ));
        if(list_empty(&(semd_target->s_procQ))){
                list_del(&semd_target->s_next);
                list_add_tail(&(semd_target->s_next), &semd_free);
            }
        return pcb_target;
    }
    return NULL;
}


/* DESCRIZIONE: Rimuove il PCB puntato da p dalla coda del
semaforo su cui è bloccato (indicato da p->p_semKey). Se il
PCB non compare in tale coda, allora restituisce NULL
(condizione di errore). Altrimenti, restituisce p. Se la coda
dei processi bloccati per il semaforo diventa vuota,
rimuove il descrittore corrispondente dalla semd_busy e lo
inserisce nella coda dei descrittori liberi (semdFree). */
pcb_t* outBlocked(pcb_t *p){
    if (p != NULL){
        struct semd_t* semd_target;
        semd_target = getSemd(p->p_semkey);
        if(semd_target != NULL){
            outProcQ(&semd_target->s_procQ, p);
            if(list_empty(&(semd_target->s_procQ))){
                list_del(&semd_target->s_next);
                list_add_tail(&(semd_target->s_next), &semd_free);
            }
            return p;
        }
        return NULL;
    }
    return NULL;
}


/* DESCRIZIONE: Restituisce (senza rimuovere) il puntatore al
PCB che si trova in testa alla coda dei processi associata al
SEMD con chiave key. Ritorna NULL se il SEMD non
compare nella ASL oppure se compare ma la sua coda dei
processi è vuota */
pcb_t* headBlocked(int *key){
    struct semd_t* tmps;
    tmps = getSemd(key);
    if(tmps == NULL || list_empty(&(tmps->s_procQ))){
        return NULL;
    } else {
        return headProcQ(&(tmps->s_procQ));
    }
}

/* DESCRIZIONE: Rimuove il PCB
puntato da p dalla coda del semaforo
su cui è bloccato (indicato da
p->p_semKey). Inoltre, elimina tutti i
processi dell’albero radicato in p (ossia
tutti i processi che hanno come avo p)
dalle eventuali code dei semafori su
cui sono bloccati. */
void outChildBlocked(pcb_t *p){
    if (p != NULL){
        if (emptyChild(p)){
            outBlocked(p);
            return;
        }
        struct list_head* tmp, *tmp2;
        tmp = p->p_child.next;
        tmp2 = &p->p_child;
        while (tmp != tmp2){
            outChildBlocked(container_of(tmp, struct pcb_t, p_sib));
            outBlocked(container_of(tmp, struct pcb_t, p_sib));
            tmp = tmp->next;
        }
        outBlocked(p);
    }
}