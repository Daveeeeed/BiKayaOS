#include "utils.h"
#include "scheduler.h"
#include "interrupt.h"
#include "syscall.h"

int main(){

    // Inizializza le New Area
    initNewArea((memaddr) SYSBK_NEWAREA, (memaddr) tlbHandler);
    initNewArea((memaddr) PGMTRAP_NEWAREA, (memaddr) tlbHandler);
    initNewArea((memaddr) TLB_NEWAREA, (memaddr) tlbHandler);
    initNewArea((memaddr) INT_NEWAREA, (memaddr) tlbHandler);
    tprint("Fatto\n");

    // Inizializza la pcb free
    initPcbs();

    // Alloca 3 pcb
    first_t = allocPcb();
    second_t = allocPcb();
    third_t = allocPcb();

    // Istanzia i 3 processi di test nei pcb
    create_process((memaddr) test1, first_t, FIRST_PRIORITY);
    //create_process((memaddr) test2, second_t, SECOND_PRIORITY);
    //create_process((memaddr) test3, third_t, THIRD_PRIORITY);

    //current_process = NULL;

    // Avvia gestione processi
    //scheduler();
    return 0;
}