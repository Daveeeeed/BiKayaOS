#include "listx.h"
#include "pcb.h"
#include "const.h"

#define HIDDEN static

HIDDEN LIST_HEAD(pcb_free);

HIDDEN pcb_t pcb_table[MAXPROC];

/* OKAY */
void initPcbs(void){
   for (int i = 0; i < MAXPROC; i++){
       list_add(&pcb_table[i].p_next, &pcb_free);
   }
}

/* OKAY */
void freePcb(pcb_t *p){
   list_add_tail(&p->p_next, &pcb_free); 
}

/* OKAY */
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

/* OKAY */
void mkEmptyProcQ(struct list_head *head){
    INIT_LIST_HEAD(head);
}

/* OKAY */
int emptyProcQ(struct list_head *head){
    return (list_empty(head));
}

/* DA CONTROLLARE */
void insertProcQ(struct list_head *head, pcb_t *p){
    struct pcb_t *test1;
    test1 = container_of(head->next, struct pcb_t, p_next);
    if(p->priority > test1->priority ||  p->priority == test1->priority || test1->priority == NULL ){
        list_add(&(p->p_next), head);
    } else {
       struct list_head *transm;
       transm = (head->next);
       insertProcQ(transm,p);
    }
}

/* DA CONTROLLARE */
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

/* DA CONTROLLARE */
pcb_t *removeProcQ(struct list_head *head){
    pcb_t *ret; 
    ret = headProcQ(head);
    list_del(head->next);
    return ret;
}

/* DA CONTROLLARE */
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

//true se p non ha figli, false se ne ha almeno uno
int emptyChild(struct pcb_t *p) {
    if( p->p_child.next == NULL ) return 1; // un list_head i cui campi puntano a NULL si intende come una lista vuota
    else return list_empty( &(p->p_child) );
}
 

//Rende p un figlio di parent, inserendolo nella sua lista di processi figli
void insertChild(struct pcb_t *parent, struct pcb_t *p) {
    if (parent == NULL || p == NULL) return;
    if( parent->p_child.next == NULL ) {	//Se i puntatori sono ancora a NULL li inizializziamo correttamente
        INIT_LIST_HEAD( &parent->p_child );
    }
    list_add_tail( &(p->p_sib), &(parent->p_child) ); 
    p->p_parent = parent;
}


//Rimuove p dalla lista dei figli di suo padre
struct pcb_t *outChild(struct pcb_t *p) {
    if ( p == NULL || p->p_parent == NULL ) return NULL; 
    list_del( &(p->p_sib) );   
    p->p_parent = NULL;
    return p;
}


 
//Rimuove il primo processo figlio di p
struct pcb_t *removeChild(struct pcb_t *p) {
    if ( p == NULL || emptyChild(p) ) return NULL;
    struct pcb_t *removedChild;
    struct list_head *q = &p->p_child;
    if (q == NULL || list_empty(q)) return NULL; //return NULL se q è NULL o la lista dei figli di p è vuota
    removedChild = container_of( q->next, typeof(*removedChild), p_sib );
    list_del(q->next);
    removedChild->p_parent = NULL;
    return removedChild;
}