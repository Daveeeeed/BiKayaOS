#include "scheduler.h"
#include "utils.h"

void scheduler(){
    // Se ci sono processi ready e il processo corrente è NULL
    if (current == NULL && !emptyProcQ(getQueue())){
        current = removeProcQ(getQueue());
        timer_on = 1;
        setTIMER(TIME_SLICE);
    // Se deve essere eseguito un context switch
    } else if (timer_on == 0){
        unsigned first_priority = 0;
        pcb_t *tmp = NULL;
        struct list_head* iterator;
        list_for_each(iterator,getQueue()){
            tmp = container_of(iterator, pcb_t, p_next);
            if (tmp->priority != 0) tmp->priority++;
            if (first_priority == 0) first_priority = tmp->priority;
        }
        current->priority = current->original_priority;
        if (current->priority < first_priority){
            insertProcQ(getQueue(), current);
            current = removeProcQ(getQueue());
        }
        timer_on = 1;
        setTIMER(TIME_SLICE);
    }
    // TIME CONTROLLER
    if (current->start_time == 0){
        current->start_time = getTODLO();
    } else {
        current->kernel_time = current->kernel_time + (getTODLO() - last_kernel_switch);
    }
    // Switch da kernel mode a user mode
    last_user_switch = getTODLO();
    LDST(&current->p_s);
}