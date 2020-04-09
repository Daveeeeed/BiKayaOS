#include "scheduler.h"
#include "utils.h"

void scheduler(){
    if (current_process == NULL && !emptyProcQ(getQueue())){
        current_process = removeProcQ(getQueue());
        timer_on = 1;
        breakpoint();
        setTIMER(TIME_SLICE);
    }
    if (current_process == NULL && emptyProcQ(getQueue())){
        HALT();
    }
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
    LDST(&current_process->p_s);
}