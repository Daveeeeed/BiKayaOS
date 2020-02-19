#include "pcb.h"
#include "const.h"

/* FUNCTION DESCRIPTION */
void initPcbs(void){

}

/* FUNCTION DESCRIPTION */
void freePcb(pcb_t *p){

}

/* FUNCTION DESCRIPTION */
pcb_t *allocPcb(void){

}

/* FUNCTION DESCRIPTION */
void mkEmptyProcQ(struct list_head *head){

}

/* FUNCTION DESCRIPTION */
int emptyProcQ(struct list_head *head){

}

/* FUNCTION DESCRIPTION */
void insertProcQ(struct list_head *head, pcb_t *p){

}

/* FUNCTION DESCRIPTION */
pcb_t *headProcQ(struct list_head *head){

}

/* FUNCTION DESCRIPTION */
pcb_t *removeProcQ(struct list_head *head){

}

/* FUNCTION DESCRIPTION */
pcb_t *outProcQ(struct list_head *head, pcb_t *p){

}


/* FUNCTION DESCRIPTION */
int emptyChild(pcb_t *this){

}

/* FUNCTION DESCRIPTION */
void insertChild(pcb_t *prnt, pcb_t *p){

}

/* FUNCTION DESCRIPTION */
pcb_t *removeChild(pcb_t *p){

}

/* FUNCTION DESCRIPTION */
pcb_t *outChild(pcb_t *p){

}
