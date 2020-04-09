#include "syscall.h"
#include "scheduler.h"
#include "utils.h"
#include "syscall.h"
#include "scheduler.h"
#include "utils.h"

#ifdef TARGET_UMPS

#define SYS_REG reg_a0

#elif defined(TARGET_UARM)

#define SYS_REG a1

#endif

void sys_handler(){

    state_t *old_state = (state_t*) SYSBK_OLDAREA;
    unsigned arg_sys = old_state->SYS_REG;
    switch(arg_sys){
        case TERMINATEPROCESS:
            terminate_process();
            break;
        default:
            tprint("System Call non riconosciuta\n");
            break;
    }
    scheduler();
    return;
    }

void terminate_process(){
    if(emptyChild(current_process)){
        freePcb(current_process);
        current_process = NULL;
    } else {
        removeChild(current_process);
        terminate_process();
    }
}