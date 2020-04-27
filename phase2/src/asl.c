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

// Ritorna il valore della chiave del SEMD contenente target
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
        INIT_LIST_HEAD( &semd_table[i].s_procQ ); //inizializza s_procQ come sentinella della lista di PCB
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
        pcb_target->p_semkey = NULL;
        if(list_empty(&(semd_target->s_procQ))){
                list_del(&semd_target->s_next);
                list_add_tail(&(semd_target->s_next), &semd_free);
            }
        return pcb_target;
    }
    return NULL;
}

pcb_t* outBlocked(pcb_t *p){
    if (p != NULL){
        struct semd_t* target;
        target = getSemd(p->p_semkey);
        if(target != NULL){
            outProcQ(&target->s_procQ, p);
            if(list_empty(&(target->s_procQ))){
                list_del(&target->s_next);
                list_add_tail(&(target->s_next), &semd_free);
            }
            return p;
        }
        return NULL;
    }
    return NULL;
}

pcb_t* headBlocked(int *key){
    struct semd_t* target;
    target = getSemd(key);
    if(target == NULL || list_empty(&(target->s_procQ))){
        return NULL;
    } else {
        return headProcQ(&(target->s_procQ));
    }
}

void outChildBlocked(pcb_t *p){
    if (p != NULL){
        if (emptyChild(p)){
            outBlocked(p);
            return;
        }
        struct list_head* iterator, *end;
        iterator = p->p_child.next;
        end = &p->p_child;
        while (iterator != end){
            outChildBlocked(container_of(iterator, struct pcb_t, p_sib));
            outBlocked(container_of(iterator, struct pcb_t, p_sib));
            iterator = iterator->next;
        }
        outBlocked(p);
    }
}

struct list_head getSemdFree(){
    return semd_free;
}