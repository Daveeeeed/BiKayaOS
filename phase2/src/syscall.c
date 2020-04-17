#include "syscall.h"
#include "scheduler.h"
#include "utils.h"
#include "syscall.h"
#include "scheduler.h"
#include "utils.h"

void sysHandler(){
    // TIME CONTROLLER
    current->user_time = current->user_time + (getTODLO() - current->last_user_switch);
    current->last_kernel_switch = getTODLO();

    state_t *old_state = (state_t*) SYSBK_OLDAREA;
    unsigned arg0,arg1,arg2,arg3;
    #ifdef TARGET_UMPS
    arg0 = old_state->reg_a0;
    arg1 = old_state->reg_a1;
    arg2 = old_state->reg_a2;
    arg3 = old_state->reg_a3;
    #elif defined(TARGET_UARM)
    arg0 = old_state->a1;
    arg1 = old_state->a2;
    arg2 = old_state->a3;
    arg3 = old_state->a4;
    #endif
    switch(arg0){
        case GETCPUTIME:
            getCpuTime((unsigned*) arg1, (unsigned*) arg2, (unsigned*) arg3);
            break;
        case CREATEPROCESS:
            createProcess((state_t*) arg1, (int) arg2, (void**) arg3);
            break;
        case TERMINATEPROCESS:
            terminateProcess((void*) arg1);
            break;
        case VERHOGEN:
            verhogen((int*) arg1);
            break;
        case PASSEREN:
            passeren((int*) arg1);
            break;
        case WAITIO:
            waitIO(arg1, (unsigned*) arg2, (int) arg3);
            break;
        case SPECPASSUP:
            specPassup((int) arg1, (state_t*) arg2, (state_t*) arg3);
            break;
        case GETPID:
            getPid((void**) arg1, (void**) arg2);
            break;
        default:
            tprint("System Call non riconosciuta o gestita\n");
            terminateProcess(0); /* Termina il processo corrente */
            break;
    }
    scheduler();
    return;
}

// TODO: controllare wallclock
void getCpuTime(unsigned int *user, unsigned int *kernel, unsigned int *wallclock){
    *wallclock = getTODLO() - current->start_time;
    user = &(current->user_time);
    kernel = &(current->kernel_time);
}

void createProcess(state_t *statep, int priority, void **cpid){
    struct pcb_t *proc_blk;
    if((proc_blk = allocPcb()) == NULL) return -1;
    if (statep == NULL )
    copyState(statep, &proc_blk->p_s);
    // Imposta le priorità
    proc_blk->priority = priority;
    proc_blk->original_priority = priority;
    // Inserisce l'indirizzo del processo nella mappa dei processi attivi
    proc_map[0]++;
    proc_map[proc_map[0]] = proc_blk;
    // Imposta i timer
    proc_blk->start_time = 0;
    proc_blk->user_time = 0;
    proc_blk->kernel_time = 0;
    // Inserisce il processo nella ready queue
    insertProcQ(getQueue(), proc_blk);
    // inserisce il processo come figlio del corrente
    insertChild(current, proc_blk);
    *cpid = proc_blk;
}

void terminateProcess(void *pid){
    if(emptyChild(current)){
        freePcb(current);
        current = NULL;
    } else {
        removeChild(current);
        terminateProcess(pid);
    }
}

void verhogen(int *semaddr){
    struct pcb_t* tmp;
    semaddr++;
    tmp = headBlocked(semaddr);
    if(semaddr >= 0 && tmp != NULL){
        tmp = removeBlocked(semaddr);
        insertProcQ(getQueue(), tmp);
    }
}

void passeren(int *semaddr){
    semaddr--;
    if(semaddr < 0){
        insertBlocked(semaddr, current);
    }
}

void waitIO(unsigned int command, unsigned int *reg, int subdevice){
    return 0;
}

void specPassup(int type, state_t *old_st, state_t *new_st){
    return 0;
}

void getPid(void **pid, void **ppid){
    *pid = current;
    *ppid = current->p_parent;
}