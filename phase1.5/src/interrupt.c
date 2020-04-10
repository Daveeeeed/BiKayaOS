#include "interrupt.h"
#include "utils.h"
#include "scheduler.h"

void disk_handler(){
    tprint("Disk interrupt handler\n");
}

void tape_handler(){
    tprint("Tape interrupt handler\n");
}

void network_handler(){
    tprint("Network interrupt handler\n");
}

void printer_handler(){
    tprint("Printer interrupt handler\n");
}

// TODO: da scrivere per uMPS
void terminal_handler(){
    memaddr* interrupt_bitmap = (memaddr*) CDEV_BITMAP_ADDR(IL_TERMINAL);
    int device_nr = get_device_nr(*interrupt_bitmap);
    if (device_nr < 0) PANIC();
    termreg_t* term = (termreg_t*) DEV_REG_ADDR(IL_TERMINAL, device_nr);
    if ((term->recv_status & TERM_STATUS_MASK) == ST_TRANS_RECV){
        term->recv_command = CMD_ACK;
    }
    if ((term->transm_status & TERM_STATUS_MASK) == ST_TRANS_RECV){
        term->transm_command = CMD_ACK;
    }
}

// DA CONTROLLARE
int get_device_nr(unsigned bitmap){

    int i;
    for (i = 0; i < 8; i++){
        if (bitmap == 1) return i;
        else bitmap = bitmap >> 1;
    }
    return -1;
}

void int_handler(){
    /*
    unsigned cause;
    state_t* old_state = (state_t*) INT_OLDAREA;
    PC(old_state);
    UPDATE_STATE(old_state, &current_process->p_s);
    GCAUSE(cause, old_state);
    if(CAUSE_IP_GET(cause, IL_TIMER)){
        timer_on = 0;
    } else if(CAUSE_IP_GET(cause, IL_DISK)){
        disk_handler();
    } else if(CAUSE_IP_GET(cause, IL_TAPE)){
        tape_handler();
    } else if(CAUSE_IP_GET(cause, IL_ETHERNET)){
        network_handler();
    } else if(CAUSE_IP_GET(cause, IL_PRINTER)){
        printer_handler();
    } else if(CAUSE_IP_GET(cause, IL_TERMINAL)){
        terminal_handler();
    } else{
        tprint("Raised unknown interrupt\n");
        PANIC();
    }
    scheduler();
    */
   unsigned cause;
   state_t* old_state = (state_t*) INT_OLDAREA;
   #ifdef TARGET_UMPS
   old_state->pc_epc = old_state->pc_epc - 4;
   cause = old_state->cause;
   #elif defined(TARGET_UARM)
   old_state->pc = old_state->pc - 4;
   cause = old_state->CP15_Cause;
   #endif
   copyState(old_state, &current_process->p_s);
   timer_on = 0;
   scheduler();
}