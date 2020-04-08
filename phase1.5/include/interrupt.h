#ifndef _INTERRUPT_H
#define _INTERRUPT_H

#ifdef TARGET_UMPS

#endif

#ifdef TARGET_UARM

#endif

#define ST_NOT_INSTALLED   0
#define ST_READY           1
#define ST_ILLEGAL_OP      2
#define ST_BUSY            3
#define ST_ERROR           4
#define ST_TRANS_RECV      5

#define CMD_RESET          0
#define CMD_ACK            1
#define CMD_TRANS_RECV     2

#define CHAR_OFFSET        8
#define TERM_STATUS_MASK   0xFF

// DTNP handler
void disk_handler();
void tape_handler();
void network_handler();
void printer_handler();

// Terminal handler
void terminal_handler();

// Data una bitmap restituisce l'indice del device che ha sollevato un interrupt
int get_device_nr(unsigned bitmap);

// Gestore di interrupt
void int_handler();

#endif

