#include "listx.h"
#include "pcb.h"
#include "const.h"

#define HIDDEN static

HIDDEN LIST_HEAD(pcb_free);

HIDDEN pcb_t pcb_table[MAXPROC];

void initPcbs(void){
   for (int i = 0; i < MAXPROC; i++){
       list_add(&pcb_table[i].p_next, &pcb_free);
   }
}

void freePcb(pcb_t *p){
   list_add_tail(&p->p_next, &pcb_free); 
}

pcb_t *allocPcb(void){
    if(list_empty(&pcb_free)){
        return NULL;
    } else {
        struct pcb_t *deleted;
        deleted = container_of(pcb_free.next, pcb_t, p_next);
        list_del(pcb_free.next);
        deleted->p_parent = NULL;
        deleted->p_semkey = NULL;
        deleted->priority = 0;
        return deleted;
    }
}

void mkEmptyProcQ(struct list_head *head){
    INIT_LIST_HEAD(head);
}

int emptyProcQ(struct list_head *head){
    return (list_empty(head));
}

void insertProcQ(struct list_head *q, struct pcb_t *p){ 
    if ( q == NULL || p == NULL)
        return;

    struct list_head *aux, *old;
    struct pcb_t *tmp;

    old = q;
    aux = q->next;

    while (aux != q && aux != NULL) {
        tmp = container_of(aux, struct pcb_t, p_next);
        if (tmp->priority < p->priority)
            break;

        old = aux;
        aux = aux->next;
    }
        
    if (aux == q)
        list_add_tail( &(p->p_next) , q );
    else
        list_add( &(p->p_next) , old );

}

pcb_t *headProcQ(struct list_head *head){
    struct pcb_t *test1;
    test1 = container_of(head->next, struct pcb_t, p_next);
    /*if(test1->priority == NULL){
        return NULL
    }                                   // potrebbe essere un bel metodo ma non ho la certezza che non crei problemi alla memoria 
                                        // discuterne con davide
    else{
        return test1;
    }*/
    if(emptyProcQ(head)) return NULL;
    else return test1;
}

pcb_t *removeProcQ(struct list_head *head){
    pcb_t *ret; 
    ret = headProcQ(head);
    list_del(head->next);
    return ret;
}

pcb_t *outProcQ(struct list_head *head, pcb_t *p){
    if(emptyProcQ(head)){
        return FALSE;
    } else {
        if(p==NULL){
            return NULL;
        } else {
            struct pcb_t *test1;
            struct list_head *test2;
            test2 = head->next; 
            test1 = container_of(test2,struct pcb_t, p_next);
            while(test1 != p && test2 != head){
                test2 = test2->next ;
                test1 = container_of(test2,struct pcb_t, p_next);
            }
            if(test1 == p){
                list_del(&(test1->p_next));
                return test1;
            } else {
                return NULL;
            }
        }
    }
}

int emptyChild(pcb_t *this){
    if (this != NULL){
        if (this->p_child.next == NULL){
            return TRUE;
        } else {
            return list_empty(&this->p_child);
        }
    }
    return ERROR;
}
 
void insertChild(pcb_t *prnt, pcb_t *p){
    if (prnt != NULL && p != NULL){
        if (emptyChild(prnt)) INIT_LIST_HEAD(&prnt->p_child);
        list_add_tail(&p->p_sib, &prnt->p_child);
    }
    p->p_parent = prnt;
}

struct pcb_t *outChild(struct pcb_t *p) {
    if ( p == NULL || p->p_parent == NULL ) return NULL; 
    list_del( &(p->p_sib) );   
    p->p_parent = NULL;
    return p;
}

pcb_t* removeChild(pcb_t *p){ 
    if(p == NULL) return NULL;
    if(emptyChild(p)) return NULL;
    pcb_t *target = container_of(p->p_child.next, pcb_t, p_sib);
    list_del(&target->p_sib);
    target->p_parent = NULL;
    return target;
}