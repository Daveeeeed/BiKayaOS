#include "utils.h"

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