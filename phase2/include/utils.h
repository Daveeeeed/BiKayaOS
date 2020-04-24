#ifndef _UTILS_H
#define _UTILS_H

#include "const.h"
#include "pcb.h"
#include "asl.h"
#include "const_bikaya.h"

// Definizoni mancanti
#define TOD_LO     *((unsigned int *)BUS_REG_TOD_LO)
#define TIME_SCALE *((unsigned int *)BUS_REG_TIME_SCALE)
#define RAMBASE    *((unsigned int *)BUS_REG_RAM_BASE)
#define RAMSIZE    *((unsigned int *)BUS_REG_RAM_SIZE)
#define RAMTOP     (RAMBASE + RAMSIZE)

// Definizioni per gestori interrupt mancanti
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
#include "umps/types.h"
#include "umps/cp0.h"

// Old e new area non definite su uMPS
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

// Ridefinizioni per architettura uMPS
#define CAUSE_IP_GET(cause,line) (cause & CAUSE_IP_MASK) & CAUSE_IP(line)

#define BUS_TODLOW  0x1000001c
#define getTODLO() (*((unsigned int *)BUS_TODLOW))

#define TIMER_USED IL_CPUTIMER

#define FIRST_ADDR_TERMINAL 0x10000250
#define FIRST_ADDR_PRINTER  0x100001D0
#define FIRST_ADDR_UNUSED   0x10000150
#define FIRST_ADDR_TAPE     0x100000D0
#define FIRST_ADDR_DISK     0x10000050

#elif defined(TARGET_UARM)
#include "uarm/libuarm.h"
#include "uarm/arch.h"
#include "uarm/uARMtypes.h"

#define FIRST_ADDR_TERMINAL 0x240
#define FIRST_ADDR_PRINTER  0x1C0
#define FIRST_ADDR_UNUSED   0x140
#define FIRST_ADDR_TAPE     0xC0
#define FIRST_ADDR_DISK     0x40

#define TIMER_USED IL_TIMER
#endif

// Funzioni esterne
extern void print(char *str);

// Tempo dedicato ad ogni processo
#define TIME_SLICE 3000

// Device complessivi (ogni terminale ha due sottodevice)
#define MAX_DEV (DEV_PER_INT * (DEV_USED_INTS + 1))

// Codici per handler alternativi
#define SYSBK_TYPE  0
#define TLB_TYPE    1
#define TRAP_TYPE   2

// Bool timer attivato
unsigned int timer_on;

// Mappa degli indirizzi dei processi attivi
unsigned proc_map[MAXPROC + 1];

// Semafori dei devices
unsigned dev_sem[MAX_DEV];

// Status in attesa di essere cominicato alla WAITIO
unsigned dev_response[MAX_DEV];

unsigned last_user_switch,last_kernel_switch;

// Processo attivo
pcb_t *current;

// Funzione di debug
void breakpoint();
void idle();

// Ritorna la coda dei processi attivi
struct list_head* getQueue();

// Inizializza una new area con l'handler ad essa associato
void initNewArea(memaddr new_area, memaddr handler);

// Inizializza un pcb e il processo ad esso associato
void initProcess(memaddr entry_point, unsigned priority);

// Copia lo stato di src dentro dest
void copyState(state_t* src, state_t* dest);

#endif