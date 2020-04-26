#include "syscall.h"
#include "scheduler.h"
#include "utils.h"
#include "exception.h"

void sysHandler(){
    
    // TIME CONTROLLER
    current->time[USERTIME] = current->time[USERTIME] + (getTODLO() - last_user_switch);
    last_kernel_switch = getTODLO();

    state_t *old_state = (state_t*) SYSBK_OLDAREA;

    copyState(old_state, &current->p_s);

    #ifdef TARGET_UMPS
    current->p_s.pc_epc = current->p_s.pc_epc + 4;
    #endif
    
    unsigned arg1 = current->p_s.ARG1;
    unsigned arg2 = current->p_s.ARG2;
    unsigned arg3 = current->p_s.ARG3;
    unsigned arg4 = current->p_s.ARG4;

    switch(arg1){
        case GETCPUTIME:
            getCpuTime((unsigned*) arg2, (unsigned*) arg3, (unsigned*) arg4);
            break;
        case CREATEPROCESS:
            createProcess((state_t*) arg2, (int) arg3, (void**) arg4);
            break;
        case TERMINATEPROCESS:
            terminateProcess((void*) arg2);
            break;
        case VERHOGEN:
            verhogen((int*) arg2);
            break;
        case PASSEREN:
            passeren((int*) arg2);
            break;
        case WAITIO:
            waitIO(arg2, (unsigned*) arg3, (int) arg4);
            break;
        case SPECPASSUP:
            specPassup((int) arg2, (state_t*) arg3, (state_t*) arg4);
            break;
        case GETPID:
            getPid((void**) arg2, (void**) arg3);
            break;
        default:
            specialHandler(SYSBK_TYPE);
            break;
    }
    scheduler();
    return;
}

void freeChildren(pcb_t *pid){
    while(!emptyChild(pid)) freeChildren(removeChild(pid));
    freePcb(pid);
}

void getCpuTime(unsigned int *user, unsigned int *kernel, unsigned int *wallclock){
    if (wallclock!= NULL) *wallclock = getTODLO() - current->time[STARTTIME];
    if (user != NULL) user = &(current->time[USERTIME]);
    if (kernel != NULL) kernel = &(current->time[KERNELTIME]);
}

void createProcess(state_t *statep, int priority, void **cpid){
    struct pcb_t *proc_blk;
    if((proc_blk = allocPcb()) == NULL || statep == NULL){
        // TODO CHECK
        if (current != NULL) current->p_s.RET_VAL = -1;
        return;
    }
    copyState(statep, &proc_blk->p_s);
    // Imposta le priorità
    proc_blk->priority = priority;
    proc_blk->original_priority = priority;
    // Inserisce l'indirizzo del processo nella mappa dei processi attivi
    proc_map[0]++;
    proc_map[proc_map[0]] = (unsigned) proc_blk;
    // Imposta i timer e le old/new area
    for(int i = 0; i < 3; i++) proc_blk->time[i] = 0;
    for(int i = 0; i < 6; i++) proc_blk->proc_area[i] = NULL;
    // Inserisce il processo nella ready queue
    insertProcQ(getQueue(), proc_blk);
    // inserisce il processo come figlio del corrente
    insertChild(current, proc_blk);
    if (cpid != NULL) *cpid = proc_blk;
    // TODO CHECK
    if (current != NULL) current->p_s.RET_VAL = 0;
    return;
}

void terminateProcess(void *pid){
    int i = 1;
    if (pid == NULL) pid = current;
    while(i <= proc_map[0]){
        if ((unsigned) pid == proc_map[i]){
            outChildBlocked(pid);
            freeChildren(pid);
            if (pid == current) current = NULL;
            // TODO CHECK
            if (current != NULL) current->p_s.RET_VAL = 0;
            return;
        }
        i++;
    }
    // TODO CHECK
    if (current != NULL) current->p_s.RET_VAL = -1;
    return;
}

void verhogen(int *semaddr){
    (*semaddr)++;
    struct pcb_t* tmp = headBlocked(semaddr);
    if(*semaddr >= 0 && tmp != NULL){
        tmp = removeBlocked(semaddr);
        insertProcQ(getQueue(), tmp);
    }
    return;
}

void passeren(int *semaddr){
    (*semaddr)--;
    if(*semaddr < 0){
        insertBlocked(semaddr, current);
        current = NULL;
    }
    return;
}

void waitIO(unsigned int command, unsigned int *reg, int subdevice){
    // ottengo il registro del comando del device
    unsigned *cmd_reg = reg + 3 - 2 * subdevice;
    // attivo il comando
    *cmd_reg = command;
    // ottengo l'indice del device
    int i = deviceIndex(reg, subdevice);
    // se non c'è nessuna risposta in attesa di essere trasmessa
    // blocco il processo sul semaforo del device
    if (dev_response[i] == 0){
        passeren(&dev_sem[i]);
    // se c'è una risposta pronta per essere trasmessa la ritorno
    } else {
        // TODO CHECK
        if (current != NULL) current->p_s.RET_VAL = dev_response[i];
        dev_response[i] = 0;
    }
    return;
}

void specPassup(int type, state_t *old_st, state_t *new_st){
    int i = type * 2;
    // se type è nel range e se la new_area è NULL
    if(type >= 0 && type < 3 && current->proc_area[i] == NULL){
        // new_area
        current->proc_area[i] = new_st;
        // old_area
        current->proc_area[i + 1] = old_st;
    } else {
        terminateProcess(NULL);
        // TODO CHECK
        if (current != NULL) current->p_s.RET_VAL = -1;
        return;
    }
    // TODO CHECK
    if (current != NULL) current->p_s.RET_VAL = 0;
    return;
}

void getPid(void **pid, void **ppid){
    if (pid != NULL) *pid = current;
    if (ppid != NULL) *ppid = current->p_parent;
    return;
}