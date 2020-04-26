#include "exception.h"
#include "syscall.h"
#include "utils.h"
#include "scheduler.h"

void trapHandler(){
    // TIME CONTROLLER
    if (current != NULL){
        current->time[USERTIME] = current->time[USERTIME] + (getTODLO() - last_user_switch);
        last_kernel_switch = getTODLO();
        specialHandler(TRAP_TYPE);
    }
}

void tlbHandler(){
    // TIME CONTROLLER
    if (current != NULL){
        current->time[USERTIME] = current->time[USERTIME] + (getTODLO() - last_user_switch);
        last_kernel_switch = getTODLO();
        specialHandler(TLB_TYPE);
    }
}

void specialHandler(unsigned type){
    int i = type * 2;
    state_t *old_state;
    // se type è nel range e se la new_area è popolata
    if (type >= 0 && type < 3 && current->proc_area[i] != NULL){
        // ottengo l'old_area
        switch (type){
        case SYSBK_TYPE:
            old_state = (state_t*) SYSBK_OLDAREA;
            #ifdef TARGET_UMPS
            old_state->pc_epc = old_state->pc_epc + 4;
            #endif
            break;
        case TLB_TYPE:
            old_state = (state_t*) TLB_OLDAREA;
            break;
        case TRAP_TYPE:
            old_state = (state_t*) PGMTRAP_OLDAREA;
            break;
        default:
            terminateProcess(NULL);
            scheduler();
            break;
        }
        copyState(old_state, current->proc_area[i + 1]);
        LDST(current->proc_area[i]);
    } else {
        terminateProcess(NULL);
        scheduler();
    }
}