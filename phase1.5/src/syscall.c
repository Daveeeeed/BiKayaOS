#include "syscall.h"
#include "scheduler.h"
#include "utils.h"
#include "syscall.h"
#include "scheduler.h"
#include "utils.h"

int sysHandler(){
    state_t *old_state = (state_t*) SYSBK_OLDAREA;
    unsigned arg_sys;
    #ifdef TARGET_UMPS
    arg_sys = old_state->reg_a0;
    #elif defined(TARGET_UARM)
    arg_sys = old_state->a1;
    #endif
    switch(arg_sys){
        case TERMINATE_PROCESS:
            terminateProcess();
            break;
        default:
            tprint("System Call non riconosciuta o gestita\n");
            break;
    }
    scheduler();
    return 2;
}

int terminateProcess(){
    if(emptyChild(current_process)){
        freePcb(current_process);
        current_process = NULL;
    } else {
        removeChild(current_process);
        terminateProcess();
    }
    return 1;
}
