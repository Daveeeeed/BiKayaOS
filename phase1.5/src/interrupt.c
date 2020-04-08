#include "interrupt.h"
#include "utils.h"
#include "scheduler.h"

#ifdef TARGET_UMPS

#endif

#ifdef TARGET_UARM

#endif

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

void terminal_handler(){
    /*
    memaddr* interrupt_bitmap = (memaddr*) CDEV_BITMAP_ADDR(INT_TERMINAL);
    int device_nr = get_device_nr(*interrupt_bitmap);
    if (device_nr < 0) PANIC();
    termreg_t* term = (termreg_t*) DEV_REG_ADDR(INT_TERMINAL, device_nr);
    if ((term->recv_status & TERM_STATUS_MASK) == DEV_TRCV_S_CHARRECV){
        term->recv_command = DEV_C_ACK;
    }
    if ((term->transm_status & TERM_STATUS_MASK) == DEV_TTRS_S_CHARTRSM){
        term->transm_command = DEV_C_ACK;
    }
    */
}

int get_device_nr(unsigned bitmap){
    /*
    int i;
    for (i = 0; i < 8; i++){
        if (bitmap == 1) return i;
        else bitmap = bitmap >> 1;
    }
    return -1;
    */
}

void int_handler(){
    /*
    state_t* old_state = (state_t*) INT_OLDAREA;
    old_state->pc = old_state->pc - 4;
    update_state(old_state, &current_process->p_s);
    unsigned cause = old_state->CP15_Cause;
    if(CAUSE_IP_GET(cause, INT_TIMER)){
        timer_on = 0;
    } else if(CAUSE_IP_GET(cause, INT_DISK)){
        disk_handler();
    } else if(CAUSE_IP_GET(cause, INT_TAPE)){
        tape_handler();
    } else if(CAUSE_IP_GET(cause, INT_UNUSED)){
        network_handler();
    } else if(CAUSE_IP_GET(cause, INT_PRINTER)){
        printer_handler();
    } else if(CAUSE_IP_GET(cause, INT_TERMINAL)){
        terminal_handler();
    } else{
        tprint("Raised unknown interrupt\n");
        PANIC();
    }
    scheduler();
    */
}