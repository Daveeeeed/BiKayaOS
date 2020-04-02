#ifdef TARGET_UMPS
#include "umps/libumps.h"
#include "umps/arch.h"
#endif

#ifdef TARGET_UARM
#include "uarm/libuarm.h"
#include "uarm/arch.h"
#endif

#include "const.h"
#include "listx.h"
#include "pcb.h"

#define FRAMESIZE 256

// Process control block associated to the 3 test process
pcb_t *first_t,*second_t,*third_t;

// Extern test functions located in p1.5test_bikaya_v0.c
extern void test1(void);
extern void test2(void);
extern void test3(void);

// Useless handlers
void sys_handler(){
    tprint("Risen sys_handler\n");
    return;
}

void trap_handler(){
    tprint("Risen trap_handler\n");
    return;
}

void tlb_handler(){
    tprint("Risen tlb_handler\n");
    return;
}

void int_handler(){
    tprint("Risen int_handler\n");
    return;
}

// Utility debug function
void breakpoint(){
    return;
}

// Inizializza una new area con l'handler ad essa associato
void init_area(memaddr new_area, memaddr handler){
    state_t* new_state = (state_t*) new_area;
    STST(new_state);
    // PC all'indirizzo dell'handler
    new_state->pc = handler;
    // SP a RAMTOP
    new_state->sp = RAM_TOP;
    // VM disabilitata
    new_state->CP15_Control = CP15_DISABLE_VM(new_state->CP15_Control);
    // Kernel mode ON e interrupt mascherati
    new_state->cpsr = new_state->cpsr | 0x000000DF;
}

void create_process(memaddr entry_point, pcb_t* process_block, unsigned priority){
    // Imposta priorità
    process_block->priority = process_block->original_priority = priority;
    // Imposta stato process block
    STST(&(process_block->p_s));
    &(process_block->p_s)->pc = entry_point;
    &(process_block->p_s)->sp = RAM_TOP - FRAMESIZE * priority;
    &(process_block->p_s)->CP15_Control = CP15_ENABLE_VM(&(process_block->p_s)->CP15_Control);
    
}

int main(){

    // Inizializza new areas
    init_area((memaddr) SYSBK_NEWAREA, (memaddr) sys_handler);
    init_area((memaddr) PGMTRAP_NEWAREA, (memaddr) trap_handler);
    init_area((memaddr) TLB_NEWAREA, (memaddr) tlb_handler);
    init_area((memaddr) INT_NEWAREA, (memaddr) int_handler);

    // Inizializza la lista dei pcb
    initPcbs();

    // Lista processi ready
    LIST_HEAD(ready_queue);

    // Istanzio i 3 processi di test
    create_process((memaddr) test1, first_t, 1);
    create_process((memaddr) test2, second_t, 2);
    create_process((memaddr) test3, third_t, 3);

    return 0;
}