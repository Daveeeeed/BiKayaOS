#include "interrupt.h"
#include "utils.h"
#include "scheduler.h"

// TODO: da scrivere per uARM e uMPS
void diskHandler(){
    tprint("Disk interrupt handler\n");
    HALT();
}

void tapeHandler(){
    tprint("Tape interrupt handler\n");
    HALT();
}

void networkHandler(){
    tprint("Network interrupt handler\n");
    HALT();
}

void printerHandler(){
    tprint("Printer interrupt handler\n");
    HALT();
}

// TODO: su uMPS non possono essere gestiti causa termprint
// modificare termprint o mascherare gestione interrupt terminale
void terminalHandler(){
    memaddr* interrupt_bitmap = (memaddr*) CDEV_BITMAP_ADDR(IL_TERMINAL);
    int device_nr = getDeviceNr(*interrupt_bitmap);
    if (device_nr < 0) PANIC();
    termreg_t* term = (termreg_t*) DEV_REG_ADDR(IL_TERMINAL, device_nr);
    if ((term->recv_status & TERM_STATUS_MASK) == ST_TRANS_RECV){
        term->recv_command = CMD_ACK;
    }
    if ((term->transm_status & TERM_STATUS_MASK) == ST_TRANS_RECV){
        term->transm_command = CMD_ACK;
    }
    return;
}

int getDeviceNr(unsigned bitmap){
    int i;
    for (i = 0; i < 8; i++){
        if (bitmap == 1) return i;
        else bitmap = bitmap >> 1;
    }
    return -1;
}

void intHandler(){
    unsigned cause;
    state_t* old_state = (state_t*) INT_OLDAREA;
    #ifdef TARGET_UMPS
    cause = old_state->cause;
    #elif defined(TARGET_UARM)
    old_state->pc = old_state->pc - 4;
    cause = old_state->CP15_Cause;
    #endif
    copyState(old_state, &current_process->p_s);
    if(CAUSE_IP_GET(cause, TIMER_USED)){
        timer_on = 0;
    } else if(CAUSE_IP_GET(cause, IL_DISK)){
        diskHandler();
    } else if(CAUSE_IP_GET(cause, IL_TAPE)){
        tapeHandler();
    } else if(CAUSE_IP_GET(cause, IL_ETHERNET)){
        networkHandler();
    } else if(CAUSE_IP_GET(cause, IL_PRINTER)){
        printerHandler();
    } else if(CAUSE_IP_GET(cause, IL_TERMINAL)){
        terminalHandler();
    } else{
        tprint("Raised unknown interrupt\n");
        PANIC();
    }
    scheduler();
}