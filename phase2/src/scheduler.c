#include "scheduler.h"
#include "utils.h"

int checkFirst(){
    if (!emptyProcQ(getQueue())){
        if (headProcQ(getQueue())->original_priority > current->priority) return 1;
        else return 0;
    } else {
        return 0;
    }
}

void scheduler(){
    // Se ci sono processi ready e il processo corrente è NULL
    if (current == NULL && !emptyProcQ(getQueue())){
        current = removeProcQ(getQueue());
        timer_on = 1;
        setTIMER(TIME_SLICE);
    // Se il processo corrente è NULL e nessun processo è in attesa d'esecuzione
    } else if (current == NULL && emptyProcQ(getQueue())){
        PANIC();
    // Se deve essere eseguito un context switch o
    // se è stato liberato un processo con priorità maggiore del corrente
    } else if (timer_on == 0 || checkFirst()){
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
    if (current->time[STARTTIME] == 0){
        current->time[STARTTIME] = getTODLO();
    }
    // Switch da kernel mode a user mode
    last_user_switch = getTODLO();
    LDST(&current->p_s);
}