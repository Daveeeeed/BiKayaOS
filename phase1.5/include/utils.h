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

#ifdef TARGET_UMPS

// Inclusioni
#include "umps/libumps.h"
#include "umps/arch.h"
#include "cp0.h"

// Variabili
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

// Funzioni esterne
extern void termprint(char *str);

// Definizioni funzioni secondo caratteristiche di architettura
#define tprint(str) termprint(str)
#define STATE_SETUP(state, handler) ({ \
        state->pc_epc = handler; /* Program counter a handler */ \
        state->gpr[26] = RAMTOP; /* Stack pointer a RAM_TOP */ \
        state->status = state->status & ~STATUS_VMc; /* Virtual memory OFF */ \
        state->status = state->status & ~STATUS_KUc; /* Kernel mode ON */ \
        state->status = state->status & ~STATUS_TE; /* Timer OFF */ \
        state->status = state->status & ~STATUS_IEc; /* Interrupt mascherati */ \
        breakpoint();\
        state->status = 0xABCDABCD; \
        })
#define PROCESS_SETUP(process,entry) ({})

#endif

#ifdef TARGET_UARM

#include "uarm/libuarm.h"
#include "uarm/arch.h"

#define STATE_SETUP(state, handler, RAM_TOP) ({ \
        state->pc = handler; /* Program counter a handler */ \
        state->sp = RAMTOP; /* Stack pointer a RAM_TOP */ \
        state->CP15_Control = CP15_DISABLE_VM(state->CP15_Control); /* DISABLE_VM(state) */ \
        state->cpsr = STATUS_ALL_INT_DISABLE(state->cpsr) | STATUS_SYS_MODE; /* Kernel mode ON, timer e interrupt mascherati */ \
        })
#define PROCESS_SETUP(process,entry) ({ \
        process->p_s.cpsr = STATUS_ALL_INT_ENABLE(process->p_s.cpsr) | STATUS_SYS_MODE; /* Abilito interrupt, timer e kernel mode */ \
        process->p_s.CP15_Control = CP15_DISABLE_VM(process->p_s.CP15_Control); /* VM disabilitata */ \
        process->p_s.sp = RAM_TOP - FRAMESIZE * priority; /* Imposta RAM adeguata */ \
        process->p_s.pc = entry_point; /* Imposta pc all'entry point */ \
        })

#endif

// Priorità dei processi
#define FIRST_PRIORITY  1
#define SECOND_PRIORITY 2
#define THIRD_PRIORITY  3

// 3 secondi
#define TIME_SLICE 3000

// Bool timer attivato
unsigned int timer_on;

// PCB associati ai 3 processi test
pcb_t *first_t,*second_t,*third_t;

// Processo attivo
pcb_t *current_process;

// Funzioni esterne di test collocate in p1.5test_bikaya_v0.c
extern void test1();
extern void test2();
extern void test3();

// Funzione di debug
extern void breakpoint();

// Gestori inutilmente utili
void trapHandler();
void tlbHandler();

// Ritorna coda ready
struct list_head* getQueue();

// Copia lo state old nello state new
void update_state(state_t *old_st, state_t* new_st);

// Inizializza una new area con l'handler ad essa associato
void initNewArea(memaddr new_area, memaddr handler);

// Inizializza un pcb e il processo ad esso associato
void create_process(memaddr entry_point, pcb_t* process_block, unsigned priority);

#endif