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

#define FIRST_PRIORITY 1
#define SECOND_PRIORITY 2
#define THIRD_PRIORITY 3

// Lista processi ready
LIST_HEAD(ready_queue);

// Process control block associati ai 3 processi test
pcb_t *first_t,*second_t,*third_t;

// Funzioni esterne di test collocate in p1.5test_bikaya_v0.c
extern void test1(void);
extern void test2(void);
extern void test3(void);

extern void scheduler(struct list_head* queue);

// Funzione di debug
void breakpoint(){
    return;
}

// Funzioni di gestione
extern void scheduler();

// Handler inutili
void sys_handler(){
    tprint("Risen sys_handler\n");
    breakpoint();
    return;
}

void trap_handler(){
    tprint("Risen trap_handler\n");
    breakpoint();
    return;
}

void tlb_handler(){
    tprint("Risen tlb_handler\n");
    breakpoint();
    return;
}

void int_handler(){
    tprint("Risen int_handler\n");
    breakpoint();
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
    // Kernel mode ON, timer e interrupt mascherati
    // new_state->cpsr = new_state->cpsr | (STATUS_TIMER_ID) | (STATUS_ID));
    new_state->cpsr = new_state->cpsr | 0x000000DF;
}

void create_process(memaddr entry_point, pcb_t* process_block, unsigned priority){
    // Imposta stato process block
    //STST(&process_block->p_s);
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
    breakpoint();
    // Inizializza la lista dei pcb
    initPcbs();

    // Istanzio i 3 processi di test
    create_process((memaddr) test1, first_t, FIRST_PRIORITY);
    create_process((memaddr) test2, second_t, SECOND_PRIORITY);
    create_process((memaddr) test3, third_t, THIRD_PRIORITY);

    // Gestione 
    scheduler(&ready_queue);
    return 0;
}