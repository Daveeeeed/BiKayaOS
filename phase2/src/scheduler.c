#include "scheduler.h"
#include "utils.h"

void scheduler(){
    // Se ci sono processi ready e il processo corrente è NULL
    if (current_process == NULL && !emptyProcQ(getQueue())){
        current_process = removeProcQ(getQueue());
        timer_on = 1;
        setTIMER(TIME_SLICE);
    }
    // Se non ci sono processi ready e il processo corrente è NULL
    if (current_process == NULL && emptyProcQ(getQueue())){
        HALT();
    }
    // Se deve essere eseguito un context switch
    if (timer_on == 0){
        pcb_t *tmp = NULL;
        struct list_head* iterator;
        list_for_each(iterator,getQueue()){
            tmp = container_of(iterator, pcb_t, p_next);
            tmp->priority ++;
        }
        current_process->priority = current_process->original_priority;
        insertProcQ(getQueue(), current_process);
        current_process = removeProcQ(getQueue());
        timer_on = 1;
        setTIMER(TIME_SLICE);
    }
    // TIME CONTROLLER
    // Settato start_time
    if (current_process->start_time == 0){
        current_process->start_time = getTODLO();
    } else {
        current_process->kernel_time = current_process->kernel_time + (getTODLO() - current_process->last_kernel_switch);
    }
    // Switch da kernel mode a user mode
    current_process->last_user_switch = getTODLO();
    LDST(&current_process->p_s);
}