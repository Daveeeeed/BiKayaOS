#include "scheduler.h"
#include "utils.h"

#ifdef TARGET_UMPS

#endif

#ifdef TARGET_UARM

#endif

void scheduler(){
    /*
    if (current_process == NULL && !emptyProcQ(get_queue())){
        current_process = removeProcQ(get_queue());
        timer_on = 1;
        setTIMER(TIME_SLICE);
    }
    if (current_process == NULL && emptyProcQ(get_queue())){
        HALT();
    }
    if (timer_on == 0){
        pcb_t *tmp = NULL;
        struct list_head* iterator;
        list_for_each(iterator,get_queue()){
            tmp = container_of(iterator, pcb_t, p_next);
            tmp->priority ++;
        }
        current_process->priority = current_process->original_priority;
        insertProcQ(get_queue(), current_process);
        current_process = removeProcQ(get_queue());
        timer_on = 1;
        setTIMER(TIME_SLICE);
    }
    LDST(&current_process->p_s);
    */
}