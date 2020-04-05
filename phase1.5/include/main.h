#ifndef MAIN_H
#define MAIN_H

#ifdef TARGET_UMPS
#include "umps/libumps.h"
#include "umps/arch.h"
#endif

#ifdef TARGET_UARM
#include "uarm/libuarm.h"
#include "uarm/arch.h"
#endif

#include "const.h"
#include "listx.h"
#include "pcb.h"

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
void breakpoint();

// Gestori inutilmente utili
void trap_handler();
void tlb_handler();

// Ritorna coda ready
struct list_head* get_queue();

// Copia lo state old nello state new
void update_state(state_t *old_st, state_t* new_st);

// Inizializza una new area con l'handler ad essa associato
void init_area(memaddr new_area, memaddr handler);

// Inizializza un pcb e il processo ad esso associato
void create_process(memaddr entry_point, pcb_t* process_block, unsigned priority);

#endif