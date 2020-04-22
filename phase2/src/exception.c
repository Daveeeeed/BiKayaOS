#include "exception.h"
#include "syscall.h"
#include "utils.h"
#include "scheduler.h"

void trapHandler(){
    // TIME CONTROLLER
    current->user_time = current->user_time + (getTODLO() - current->last_user_switch);
    current->last_kernel_switch = getTODLO();
    specHandler(TRAP_TYPE);
}

void tlbHandler(){
    // TIME CONTROLLER
    current->user_time = current->user_time + (getTODLO() - current->last_user_switch);
    current->last_kernel_switch = getTODLO();
    specHandler(TLB_TYPE);
}

void specHandler(unsigned type){
    if (current != NULL){
        switch (type){
        case SYSBK_TYPE:
            if (current->new_sysbk != NULL){
                STST(&current->old_sysbk);
                LDST(&current->new_sysbk);
            } else {
                terminateProcess(NULL);
            }
            break;
        case TLB_TYPE:
            if (current->new_tlb != NULL){
                STST(&current->old_tlb);
                LDST(&current->new_tlb);
            } else {
                terminateProcess(NULL);
            }
            break;
        case TRAP_TYPE:
            if (current->new_trap != NULL){
                STST(&current->old_trap);
                LDST(&current->new_trap);
            } else {
                terminateProcess(NULL);
            }
            break;
        default:
            terminateProcess(NULL);
        }
    }
    scheduler();
}