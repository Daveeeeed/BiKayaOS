#include "utils.h"

// Coda dei processi attivi
LIST_HEAD(ready_queue);

struct list_head* getQueue(){
    return &ready_queue;
}

void breakpoint(){
}

void trapHandler(){
    tprint("Risen trap handler, it's not handled so...");
    HALT();
}

void tlbHandler(){
    tprint("Risen TLB handler, it's not handled so...");
    HALT();
}

void initNewArea(memaddr new_area, memaddr handler){
    state_t* new_state = (state_t*) new_area;
    STST(new_state);
    #ifdef TARGET_UMPS
    new_state->pc_epc = handler; /* Program counter a handler */
    new_state->reg_sp = RAMTOP; /* Stack pointer a RAM_TOP */
    new_state->status = new_state->status & ~(STATUS_VMc | STATUS_VMp | STATUS_VMo); /* Virtual memory OFF */
    new_state->status = new_state->status & ~(STATUS_KUc | STATUS_KUp | STATUS_KUo); /* Kernel mode ON */
    new_state->status = new_state->status & ~STATUS_TE; /* Timer OFF */
    new_state->status = new_state->status & ~(STATUS_IEc | STATUS_IEp | STATUS_IEo); /* Interrupt mascherati */
    #elif defined(TARGET_UARM)
    new_state->pc = handler; /* Program counter  handler */
    new_state->sp = RAMTOP; /* Stack pointer a RAM_TOP */
    new_state->CP15_Control = CP15_DISABLE_VM(new_state->CP15_Control); /* DISABLE_VM(state) */
    new_state->cpsr = STATUS_ALL_INT_DISABLE(new_state->cpsr) | STATUS_SYS_MODE; /* Kernel mode ON, timer e interrupt mascherati */
    #endif
}

// Mascherare solo interrupt timer o no? Se no, fare handler anche degli altri interrupt
void createProcess(memaddr entry_point, pcb_t* process_block, unsigned priority){
    #ifdef TARGET_UMPS
    process_block->p_s.status = process_block->p_s.status & ~(STATUS_VMc | STATUS_VMp | STATUS_VMo); /* Virtual memory OFF */
    process_block->p_s.status = process_block->p_s.status & ~(STATUS_KUc | STATUS_KUp | STATUS_KUo); /* Kernel mode ON */
    process_block->p_s.status = process_block->p_s.status | STATUS_TE; /* Timer ON */
    process_block->p_s.status = process_block->p_s.status | STATUS_IEc | STATUS_IEp | STATUS_IEo; /* Interrupt abilitati */
    process_block->p_s.status = process_block->p_s.status | STATUS_IM_MASK; /* Attiva tutti gli interrupt */
    process_block->p_s.reg_sp = RAMTOP - FRAMESIZE * priority; /* Imposta RAM adeguata */
    process_block->p_s.pc_epc = entry_point; /* Imposta pc all'entry point */
    #elif defined(TARGET_UARM)
    process_block->p_s.cpsr = STATUS_ALL_INT_ENABLE(process_block->p_s.cpsr) | STATUS_SYS_MODE; /* Abilito interrupt, timer e kernel mode */
    process_block->p_s.CP15_Control = CP15_DISABLE_VM(process_block->p_s.CP15_Control); /* VM disabilitata */
    process_block->p_s.sp = RAMTOP - FRAMESIZE * priority; /* Imposta RAM adeguata */
    process_block->p_s.pc = entry_point; /* Imposta pc all'entry point */
    #endif
    // Imposta le priorità
    process_block->priority = priority;
    process_block->original_priority = priority;
    // Inserisce il processo nella lista ready
    insertProcQ(&ready_queue, process_block);
}

// TODO: verificare il range del 'for'
void copyState(state_t* src, state_t* dest){
    #ifdef TARGET_UMPS
    for(int i = 0; i < 31; i++) dest->gpr[i] = src->gpr[i];
    dest->pc_epc = src->pc_epc;
    dest->status = src->status;
    dest->entry_hi = src->entry_hi;
    dest->cause = src->cause;
    dest->hi = src->hi;
    dest->lo = src->lo;
    #elif defined(TARGET_UARM)
    dest->a1 = src->a1;
    dest->a2 = src->a2;
    dest->a3 = src->a3;
    dest->a4 = src->a4;
    dest->v1 = src->v1;
    dest->v2 = src->v2;
    dest->v3 = src->v3;
    dest->v4 = src->v4;
    dest->v5 = src->v5;
    dest->v6 = src->v6;
    dest->sl = src->sl;
    dest->fp = src->fp;
    dest->ip = src->ip;
    dest->sp = src->sp;
    dest->lr = src->lr;
    dest->pc = src->pc;
    dest->cpsr = src->cpsr;
    dest->CP15_Control = src->CP15_Control;
    dest->CP15_EntryHi = src->CP15_EntryHi;
    dest->CP15_Cause = src->CP15_Cause;
    dest->TOD_Hi = src->TOD_Hi;
    dest->TOD_Low = src->TOD_Low;
    #endif
}