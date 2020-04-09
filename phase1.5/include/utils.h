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
#define FRAMESIZE       1024

// Funzioni esterne
extern void termprint(char *str);

// Definizioni funzioni secondo caratteristiche di architettura
#define tprint(str) termprint(str)
#define STATE_SETUP(state, handler) ({ \
        state->pc_epc = handler; /* Program counter a handler */ \
        state->reg_sp = RAMTOP; /* Stack pointer a RAM_TOP */ \
        state->status = state->status & ~(STATUS_VMc | STATUS_VMp | STATUS_VMo); /* Virtual memory OFF */ \
        state->status = state->status & ~(STATUS_KUc | STATUS_KUp | STATUS_KUo); /* Kernel mode ON */ \
        state->status = state->status & ~STATUS_TE; /* Timer OFF */ \
        state->status = state->status & ~(STATUS_IEc | STATUS_IEp | STATUS_IEo); /* Interrupt mascherati */ \
        })
#define PROCESS_SETUP(process,entry,priority) ({ \
        process->p_s.status = process->p_s.status & ~(STATUS_VMc | STATUS_VMp | STATUS_VMo); /* Virtual memory OFF */ \
        process->p_s.status = process->p_s.status & ~(STATUS_KUc | STATUS_KUp | STATUS_KUo); /* Kernel mode ON */ \
        process->p_s.status = process->p_s.status | STATUS_TE; /* Timer ON */ \
        process->p_s.status = process->p_s.status | STATUS_IEc | STATUS_IEp | STATUS_IEo; /* Interrupt abilitati */ \
        process->p_s.status = process->p_s.status | STATUS_IM_MASK; \
        process->p_s.gpr[26] = RAMTOP - FRAMESIZE * priority; /* Imposta RAM adeguata */ \
        process->p_s.pc_epc = entry; /* Imposta pc all'entry point */ \
        })
#define PC(state) (state->pc_epc = state->pc_epc - 4)
#define UPDATE_STATE(state_old, state_new) ({ \
        for(int i = 0; i < 31; i++) (state_new)->gpr[i] = (state_old)->gpr[i]; \
        (state_new)->pc_epc = (state_old)->pc_epc; \
        (state_new)->status = (state_old)->status; \
        (state_new)->entry_hi = (state_old)->entry_hi; \
        (state_new)->cause = (state_old)->cause; \
        (state_new)->hi = (state_old)->hi; \
        (state_new)->lo = (state_old)->lo; \
        })
#define GCAUSE(cause,state) (cause = state->cause)

#endif

#ifdef TARGET_UARM

#include "uarm/libuarm.h"
#include "uarm/arch.h"

#define STATE_SETUP(state, handler) ({ \
        state->pc = handler; /* Program counter a handler */ \
        state->sp = RAMTOP; /* Stack pointer a RAM_TOP */ \
        state->CP15_Control = CP15_DISABLE_VM(state->CP15_Control); /* DISABLE_VM(state) */ \
        state->cpsr = STATUS_ALL_INT_DISABLE(state->cpsr) | STATUS_SYS_MODE; /* Kernel mode ON, timer e interrupt mascherati */ \
        })
#define PROCESS_SETUP(process,entry,priority) ({ \
        process->p_s.cpsr = STATUS_ALL_INT_ENABLE(process->p_s.cpsr) | STATUS_SYS_MODE; /* Abilito interrupt, timer e kernel mode */ \
        process->p_s.CP15_Control = CP15_DISABLE_VM(process->p_s.CP15_Control); /* VM disabilitata */ \
        process->p_s.sp = RAMTOP - FRAMESIZE * priority; /* Imposta RAM adeguata */ \
        process->p_s.pc = entry_point; /* Imposta pc all'entry point */ \
        })
#define PC(state) (state->pc = state->pc - 4)
#define UPDATE_STATE(old, state_new) ({ \
        (state_new)->a1 = (old)->a1; \
        (state_new)->a2 = (old)->a2; \
        (state_new)->a3 = (old)->a3; \
        (state_new)->a4 = (old)->a4; \
        (state_new)->v1 = (old)->v1; \
        (state_new)->v2 = (old)->v2; \
        (state_new)->v3 = (old)->v3; \
        (state_new)->v4 = (old)->v4; \
        (state_new)->v5 = (old)->v5; \
        (state_new)->v6 = (old)->v6; \
        (state_new)->sl = (old)->sl; \
        (state_new)->fp = (old)->fp; \
        (state_new)->ip = (old)->ip; \
        (state_new)->sp = (old)->sp; \
        (state_new)->lr = (old)->lr; \
        (state_new)->pc = (old)->pc; \
        (state_new)->cpsr = (old)->cpsr; \
        (state_new)->CP15_Control = (old)->CP15_Control; \
        (state_new)->CP15_EntryHi = (old)->CP15_EntryHi; \
        (state_new)->CP15_Cause = (old)->CP15_Cause; \
        (state_new)->TOD_Hi = (old)->TOD_Hi; \
        (state_new)->TOD_Low = (old)->TOD_Low; \
        })
#define GCAUSE(cause,state) (cause = state->CP15_Cause)

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

extern void sys_handler();

// Funzione di debug
extern void breakpoint();

// Gestori inutilmente utili
void trapHandler();
void tlbHandler();

// Ritorna coda ready
struct list_head* getQueue();

// Inizializza una new area con l'handler ad essa associato
void initNewArea(memaddr new_area, memaddr handler);

// Inizializza un pcb e il processo ad esso associato
void createProcess(memaddr entry_point, pcb_t* process_block, unsigned priority);

#endif