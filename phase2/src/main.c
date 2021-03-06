#include "utils.h"
#include "scheduler.h"
#include "interrupt.h"
#include "syscall.h"
#include "exception.h"

// Funzioni esterne di test collocate in p2test_bikaya.c
extern void test();

int main(){

    // Inizializza le New Area
    initNewArea((memaddr) SYSBK_NEWAREA, (memaddr) sysHandler);
    initNewArea((memaddr) PGMTRAP_NEWAREA, (memaddr) trapHandler);
    initNewArea((memaddr) TLB_NEWAREA, (memaddr) tlbHandler);
    initNewArea((memaddr) INT_NEWAREA, (memaddr) intHandler);

    // Inizializza la pcb free
    initPcbs();
    initASL();

    current = NULL;
    
    for (int i = 0; i < MAXDEV; i++){
        dev_response[i] = 0;
        dev_sem[i] = 0;
    }

    for (int i = 0; i <= MAXPROC; i++){
        proc_map[i] = 0;
    }

    // Istanzia i 3 processi di test nei pcb
    initProcess((memaddr) test, DEFAULT_PRIORITY);
    initProcess((memaddr) idle, 0);
    // Avvia gestione processi
    scheduler();
    return 0;
}