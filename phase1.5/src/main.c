#include "main.h"
#include "scheduler.h"
#include "interrupt.h"
#include "syscall.h"

LIST_HEAD(ready_queue);

void breakpoint(){
    return;
}

void trap_handler(){
    tprint("Risen trap handler, it's not handled so...");
    HALT();
    return;
}

void tlb_handler(){
    tprint("Risen TLB handler, it's not handled so...");
    HALT();
    return;
}

struct list_head* get_queue(){
    return &ready_queue;
}

void update_state(state_t *old, state_t *new){	
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
}

void init_area(memaddr new_area, memaddr handler){
    state_t* new_state = (state_t*) new_area;
    STST(new_state);
    // PC all'indirizzo dell'handler
    new_state->pc = handler;
    // SP a RAMTOP
    new_state->sp = RAM_TOP;
    // VM disabilitata
    new_state->CP15_Control = CP15_DISABLE_VM(new_state->CP15_Control);
    // Kernel mode ON, timer e interrupt mascherati
    new_state->cpsr = STATUS_ALL_INT_DISABLE(new_state->cpsr) | STATUS_SYS_MODE;
}

void create_process(memaddr entry_point, pcb_t* process_block, unsigned priority){
    // Abilito interrupt, timer e kernel mode
    process_block->p_s.cpsr = STATUS_ALL_INT_ENABLE(process_block->p_s.cpsr) | STATUS_SYS_MODE;
    // VM disabilitata
    process_block->p_s.CP15_Control = CP15_DISABLE_VM(process_block->p_s.CP15_Control);
    // Imposta RAM adeguata
    process_block->p_s.sp = RAM_TOP - FRAMESIZE * priority;
    // Imposta priorità
    process_block->priority = priority;
    process_block->original_priority = priority;
    // Imposta pc all'entry point
    process_block->p_s.pc = entry_point;
    // Inserisce il processo nella lista ready
    insertProcQ(&ready_queue, process_block);
}

int main(){

    // Inizializza new areas
    init_area((memaddr) SYSBK_NEWAREA, (memaddr) sys_handler);
    init_area((memaddr) PGMTRAP_NEWAREA, (memaddr) trap_handler);
    init_area((memaddr) TLB_NEWAREA, (memaddr) tlb_handler);
    init_area((memaddr) INT_NEWAREA, (memaddr) int_handler);

    // Inizializza la pcb free
    initPcbs();

    // Alloca 3 pcb
    first_t = allocPcb();
    second_t = allocPcb();
    third_t = allocPcb();

    // Istanzia i 3 processi di test nei pcb
    create_process((memaddr) test1, first_t, FIRST_PRIORITY);
    create_process((memaddr) test2, second_t, SECOND_PRIORITY);
    create_process((memaddr) test3, third_t, THIRD_PRIORITY);

    current_process = NULL;

    // Gestione
    scheduler();
    return 0;
}