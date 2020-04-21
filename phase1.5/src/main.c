#include "utils.h"
#include "scheduler.h"
#include "interrupt.h"
#include "syscall.h"

// Funzioni esterne di test collocate in p1.5test_bikaya_v0.c
extern void test1();
extern void test2();
extern void test3();

int f;

int main(){

    // Inizializza le New Area
    initNewArea((memaddr) SYSBK_NEWAREA, (memaddr) sysHandler);
    initNewArea((memaddr) PGMTRAP_NEWAREA, (memaddr) trapHandler);
    initNewArea((memaddr) TLB_NEWAREA, (memaddr) tlbHandler);
    initNewArea((memaddr) INT_NEWAREA, (memaddr) intHandler);

    // Inizializza la pcb free
    initPcbs();
    
    f = SYSCALL(TERMINATE_PROCESS, 0, 0, 0);
    breakpoint();
    // Alloca 3 pcb
    first_t = allocPcb();
    second_t = allocPcb();
    third_t = allocPcb();

    // Istanzia i 3 processi di test nei pcb
    createProcess((memaddr) test1, first_t, FIRST_PRIORITY);
    createProcess((memaddr) test2, second_t, SECOND_PRIORITY);
    createProcess((memaddr) test3, third_t, THIRD_PRIORITY);

    current_process = NULL;

    // Avvia gestione processi
    scheduler();
    return 0;
}