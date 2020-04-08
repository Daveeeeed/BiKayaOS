#include "syscall.h"
#include "scheduler.h"
#include "utils.h"

#ifdef TARGET_UMPS

#endif

#ifdef TARGET_UARM

#endif

// Gestore System Call
void sys_handler(){
    /*
    state_t* old_state = (state_t*) SYSBK_OLDAREA;
    unsigned arg_sys = old_state->a1;
    switch (arg_sys) {
        case TERMINATEPROCESS:
            terminate_process();
            break;
        default:
            tprint("System Call non riconosciuta\n");
            break;
    }
    scheduler();
    return;
    */
}

void terminate_process(){
    /*
    if(emptyChild(current_process)){
        freePcb(current_process);
        current_process = NULL;
    } else {
        removeChild(current_process);
        terminate_process();
    }
    */
}