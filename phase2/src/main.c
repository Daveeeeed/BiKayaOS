#include "utils.h"
#include "scheduler.h"
#include "interrupt.h"
#include "syscall.h"
#include "exception.h"

// Funzioni esterne di test collocate in p1.5test_bikaya_v0.c
extern void test();

int main(){

    // Inizializza le New Area
    initNewArea((memaddr) SYSBK_NEWAREA, (memaddr) sysHandler);
    initNewArea((memaddr) PGMTRAP_NEWAREA, (memaddr) trapHandler);
    initNewArea((memaddr) TLB_NEWAREA, (memaddr) tlbHandler);
    initNewArea((memaddr) INT_NEWAREA, (memaddr) intHandler);

    proc_map[0] = 0;

    // Inizializza la pcb free
    initPcbs();
    initASL();

    current = NULL;

    // Istanzia i 3 processi di test nei pcb
    initProcess((memaddr) test, FIRST_PRIORITY);

    // Avvia gestione processi
    scheduler();
    return 0;
}