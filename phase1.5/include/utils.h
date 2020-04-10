#ifndef _UTILS_H
#define _UTILS_H

#include "const.h"
#include "listx.h"
#include "pcb.h"

// DA RIMUOVERE
#define TOD_LO     *((unsigned int *)BUS_REG_TOD_LO)
#define TIME_SCALE *((unsigned int *)BUS_REG_TIME_SCALE)
#define RAMBASE    *((unsigned int *)BUS_REG_RAM_BASE)
#define RAMSIZE    *((unsigned int *)BUS_REG_RAM_SIZE)
#define RAMTOP     (RAMBASE + RAMSIZE)

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

#ifdef TARGET_UMPS

#include "umps/libumps.h"
#include "umps/arch.h"
#include "cp0.h"

#define STATE_T_SIZE    0x0000008C
#define EXCV_BASE       0x20000000
#define INT_OLDAREA     EXCV_BASE
#define INT_NEWAREA     (EXCV_BASE + STATE_T_SIZE)
#define TLB_OLDAREA     (EXCV_BASE + (2 * STATE_T_SIZE))
#define TLB_NEWAREA     (EXCV_BASE + (3 * STATE_T_SIZE))
#define PGMTRAP_OLDAREA (EXCV_BASE + (4 * STATE_T_SIZE))
#define PGMTRAP_NEWAREA (EXCV_BASE + (5 * STATE_T_SIZE))
#define SYSBK_OLDAREA   (EXCV_BASE + (6 * STATE_T_SIZE))
#define SYSBK_NEWAREA   (EXCV_BASE + (7 * STATE_T_SIZE))
#define FRAMESIZE       1024

// Funzioni esterne
extern void termprint(char *str);

// Ridefinizioni per architettura uMPS
#define tprint(str) termprint(str)

#define CAUSE_IP_GET(cause,line) (cause & CAUSE_IP_MASK) & CAUSE_IP(line)

#define TIMER_USED IL_CPUTIMER

#elif defined(TARGET_UARM)

#include "uarm/libuarm.h"
#include "uarm/arch.h"

#define TIMER_USED IL_TIMER

#endif

// Priorità dei processi
#define FIRST_PRIORITY  1
#define SECOND_PRIORITY 2
#define THIRD_PRIORITY  3

// Tempo dedicato ad ogni processo
#define TIME_SLICE 3000

// System call
#define TERMINATE_PROCESS 3

// Bool timer attivato
unsigned int timer_on;

// PCB associati ai 3 processi test
pcb_t *first_t,*second_t,*third_t;

// Processo attivo
pcb_t *current_process;

// Funzione di debug
void breakpoint();

// Gestori inutilmente utili
void trapHandler();
void tlbHandler();

// Ritorna la coda dei processi attivi
struct list_head* getQueue();

// Inizializza una new area con l'handler ad essa associato
void initNewArea(memaddr new_area, memaddr handler);

// Inizializza un pcb e il processo ad esso associato
void createProcess(memaddr entry_point, pcb_t* process_block, unsigned priority);

// Copia lo stato di src dentro dest
void copyState(state_t* src, state_t* dest);

#endif