#include "utils.h"

#ifdef TARGHET_UARM

#endif

#ifdef TARGET_UMPS

#endif

LIST_HEAD(ready_queue);

void breakpoint(){
    return;
}

void trapHandler(){
    tprint("Risen trap handler, it's not handled so...");
    HALT();
    return;
}

void tlbHandler(){
    tprint("Risen TLB handler, it's not handled so...");
    HALT();
    return;
}

struct list_head* getQueue(){
    return &ready_queue;
}

void update_state(state_t *old, state_t *new){	
	/*
    new->a1 = old->a1;
	new->a2 = old->a2;
	new->a3 = old->a3;
	new->a4 = old->a4;
	new->v1 = old->v1;
	new->v2 = old->v2;
	new->v3 = old->v3;
	new->v4 = old->v4;
	new->v5 = old->v5;
	new->v6 = old->v6;
	new->sl = old->sl;
	new->fp = old->fp;
	new->ip = old->ip;
	new->sp = old->sp;
	new->lr = old->lr;
	new->pc = old->pc;
	new->cpsr = old->cpsr;
	new->CP15_Control = old->CP15_Control;
	new->CP15_EntryHi = old->CP15_EntryHi;
	new->CP15_Cause = old->CP15_Cause;
	new->TOD_Hi = old->TOD_Hi;
	new->TOD_Low = old->TOD_Low;
    */
    return;
}

void initNewArea(memaddr new_area, memaddr handler){
    state_t* new_state = (state_t*) new_area;
    STST(new_state);
    STATE_SETUP(new_state, handler);
    return;
}

void createProcess(memaddr entry_point, pcb_t* process_block, unsigned priority){
    PROCESS_SETUP(process_block, entry_point,priority);
    // Imposta priorità
    process_block->priority = priority;
    process_block->original_priority = priority;
    // Inserisce il processo nella lista ready
    insertProcQ(&ready_queue, process_block);
   return;
}