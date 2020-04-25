#include "interrupt.h"
#include "utils.h"
#include "scheduler.h"
#include "syscall.h"

// TODO: da scrivere per uARM e uMPS
void diskHandler(){
    print("Disk interrupt handler\n");
    HALT();
}

void tapeHandler(){
    print("Tape interrupt handler\n");
    HALT();
}

void networkHandler(){
    print("Network interrupt handler\n");
    HALT();
}

void printerHandler(){
    print("Printer interrupt handler\n");
    HALT();
}

// TODO: su uMPS non possono essere gestiti causa termprint
// modificare termprint o mascherare gestione interrupt terminale
void terminalHandler(){
    int i, status, device_nr;
    termreg_t* term;
    memaddr* interrupt_bitmap = (memaddr*) CDEV_BITMAP_ADDR(INT_TERMINAL);

    if ((device_nr = getDeviceNr(*interrupt_bitmap)) < 0) PANIC();
    else term = (termreg_t*) DEV_REG_ADDR(INT_TERMINAL, device_nr);

    if ((term->recv_status & TERM_STATUS_MASK) == ST_TRANS_RECV){
        i = DEV_PER_INT * (INT_TERMINAL - 3) + device_nr;
        status = term->recv_status;
        term->recv_command = CMD_ACK;
    }
    else if ((term->transm_status & TERM_STATUS_MASK) == ST_TRANS_RECV){
        i = DEV_PER_INT * (INT_TERMINAL - 3 + 1) + device_nr;
        status = term->transm_status;
        term->transm_command = CMD_ACK;
    }

    if(dev_sem[i] < 0){
        verhogen(&dev_sem[i]);
        current->p_s.RET_VAL = status;
        dev_response[i] = 0;
    } else {
        dev_response[i] = status;
    }
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
    // TIME CONTROLLER
    current->user_time = current->user_time + (getTODLO() - last_user_switch);
    last_kernel_switch = getTODLO();

    unsigned cause;
    state_t* old_state = (state_t*) INT_OLDAREA;
    #ifdef TARGET_UMPS
    cause = old_state->cause;
    #elif defined(TARGET_UARM)
    old_state->pc = old_state->pc - 4;
    cause = old_state->CP15_Cause;
    #endif
    copyState(old_state, &current->p_s);
    if(CAUSE_IP_GET(cause, TIMER_USED)){
        timer_on = 0;
    } else if(CAUSE_IP_GET(cause, INT_DISK)){
        diskHandler();
    } else if(CAUSE_IP_GET(cause, INT_TAPE)){
        tapeHandler();
    } else if(CAUSE_IP_GET(cause, INT_UNUSED)){
        networkHandler();
    } else if(CAUSE_IP_GET(cause, INT_PRINTER)){
        printerHandler();
    } else if(CAUSE_IP_GET(cause, INT_TERMINAL)){
        terminalHandler();
    } else{
        print("Raised unknown interrupt\n");
        PANIC();
    }
    scheduler();
}