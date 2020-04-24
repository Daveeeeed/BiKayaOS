#include "syscall.h"
#include "scheduler.h"
#include "utils.h"
#include "exception.h"

void sysHandler(){
    // TIME CONTROLLER
    current->user_time = current->user_time + (getTODLO() - last_user_switch);
    last_kernel_switch = getTODLO();

    state_t *old_state = (state_t*) SYSBK_OLDAREA;
    unsigned arg0,arg1,arg2,arg3,return_v;
    #ifdef TARGET_UMPS
    arg0 = old_state->reg_a0;
    arg1 = old_state->reg_a1;
    arg2 = old_state->reg_a2;
    arg3 = old_state->reg_a3;
    return_v = old_state->reg_v0;
    old_state->pc_epc = old_state->pc_epc + 4;
    #elif defined(TARGET_UARM)
    arg0 = old_state->a1;
    arg1 = old_state->a2;
    arg2 = old_state->a3;
    arg3 = old_state->a4;
    return_v = old_state->a1;
    #endif
    copyState(old_state, &current->p_s);
    switch(arg0){
        case GETCPUTIME:
            getCpuTime((unsigned*) arg1, (unsigned*) arg2, (unsigned*) arg3);
            break;
        case CREATEPROCESS:
            return_v = createProcess((state_t*) arg1, (int) arg2, (void**) arg3);
            break;
        case TERMINATEPROCESS:
            return_v = terminateProcess((void*) arg1);
            break;
        case VERHOGEN:
            verhogen((int*) arg1);
            break;
        case PASSEREN:
            passeren((int*) arg1);
            break;
        case WAITIO:
            return_v = waitIO(arg1, (unsigned*) arg2, (int) arg3);
            break;
        case SPECPASSUP:
            return_v = specPassup((int) arg1, (state_t*) arg2, (state_t*) arg3);
            break;
        case GETPID:
            getPid((void**) arg1, (void**) arg2);
            break;
        default:
            specHandler(SYSBK_TYPE);
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
    if (wallclock!= NULL) *wallclock = getTODLO() - current->start_time;
    if (user != NULL) user = &(current->user_time);
    if (kernel != NULL) kernel = &(current->kernel_time);
}

int createProcess(state_t *statep, int priority, void **cpid){
    struct pcb_t *proc_blk;
    if((proc_blk = allocPcb()) == NULL || statep == NULL) return -1;
    copyState(statep, &proc_blk->p_s);
    // Imposta le priorità
    proc_blk->priority = priority;
    proc_blk->original_priority = priority;
    // Inserisce l'indirizzo del processo nella mappa dei processi attivi
    proc_map[0]++;
    proc_map[proc_map[0]] = (unsigned) proc_blk;
    // Imposta i timer
    proc_blk->start_time = 0;
    proc_blk->user_time = 0;
    proc_blk->kernel_time = 0;
    proc_blk->new_sysbk = NULL;
    proc_blk->new_trap = NULL;
    proc_blk->new_tlb = NULL;
    // Inserisce il processo nella ready queue
    insertProcQ(getQueue(), proc_blk);
    // inserisce il processo come figlio del corrente
    insertChild(current, proc_blk);
    if (cpid != NULL) *cpid = proc_blk;
    return 0;
}

int terminateProcess(void *pid){
    int i = 1;
    if (pid == NULL) pid = current;
    while(i <= proc_map[0]){
        if ((unsigned) pid == proc_map[i]){
            outChildBlocked(pid);
            freeChildren(pid);
            if (pid == current) current = NULL;
            return 0;
        }
        i++;
    }
    return -1;
}

void verhogen(int *semaddr){
    struct pcb_t* tmp;
    (*semaddr)++;
    tmp = headBlocked(semaddr);
    if(*semaddr >= 0 && tmp != NULL){
        tmp = removeBlocked(semaddr);
        insertProcQ(getQueue(), tmp);
    }
}

void passeren(int *semaddr){
    (*semaddr)--;
    if(*semaddr < 0){
        insertBlocked(semaddr, current);
        current = NULL;
    }
}

unsigned deviceIndex(unsigned *reg, int subdevice){
    unsigned dev_line,dev_pos,address;
    address = (unsigned)reg;
    if (address >= FIRST_ADDR_TERMINAL){
        address -= FIRST_ADDR_TERMINAL;
        if (subdevice) dev_line = INT_TERMINAL;
        else dev_line = INT_TERMINAL + 1;
    } else if (address >= FIRST_ADDR_PRINTER){
        address -= FIRST_ADDR_PRINTER;
        dev_line = INT_PRINTER;
    } else if (address >= FIRST_ADDR_UNUSED){
        address -= FIRST_ADDR_UNUSED;
        dev_line = INT_UNUSED;
    } else if (address >= FIRST_ADDR_TAPE){
        address -= FIRST_ADDR_TAPE;
        dev_line = INT_TAPE;
    } else if (address >= FIRST_ADDR_DISK){
        address -= FIRST_ADDR_DISK;
        dev_line = INT_DISK;
    }
    dev_pos = address/10;
    return (dev_line - 3) * 8 + dev_pos;
}

int waitIO(unsigned int command, unsigned int *reg, int subdevice){
    // ottengo il registro del comando del device
    unsigned *cmd_reg = reg + 3 - 2 * subdevice;
    // attivo il comando
    *cmd_reg = command;
    // ottengo l'indice del device
    int i = deviceIndex(reg, subdevice);
    // se non c'è nessuna risposta in attesa di essere trasmessa, blocco il processo sul semaforo del device
    if (dev_response[i] == 0){
        passeren(&dev_sem[i]);
    // se c'è una risposta pronta per essere trasmessa la ritorno
    } else {
        unsigned tmp = dev_response[i];
        dev_response[i] = 0;
        return tmp;
    }
}

int specPassup(int type, state_t *old_st, state_t *new_st){
    switch (type) {
    case SYSBK_TYPE:
        if(current->new_sysbk != NULL){
            current->new_sysbk = new_st;
            current->old_sysbk = old_st;
        } else {
            terminateProcess(NULL);
            return -1;
        }
        break;
    case TLB_TYPE:
        if(current->new_tlb != NULL){
            current->new_tlb = new_st;
            current->old_tlb = old_st;
        } else {
            terminateProcess(NULL);
            return -1;
        }
        break;
    case TRAP_TYPE:
        if(current->new_trap != NULL){
            current->new_trap = new_st;
            current->old_trap = old_st;
        } else {
            terminateProcess(NULL);
            return -1;
        }
        break;
    default:
        terminateProcess(NULL);
        break;
    }
    return 0;
}

void getPid(void **pid, void **ppid){
    if (pid != NULL) *pid = current;
    if (ppid != NULL) *ppid = current->p_parent;
}