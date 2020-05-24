#ifndef _SYSCALL_H
#define _SYSCALL_H

#include "types_bikaya.h"

// Gestore system call
void sysHandler();

// SYSCALL #1
void getCpuTime(unsigned int *user, unsigned int *kernel, unsigned int *wallclock);

// SYSCALL #2
void createProcess(state_t *statep, int priority, void **cpid);

// Funzione ausiliaria per SYSCALL #3, dato un pcb_t ne elimina l'albero radicato
// root: pcb_t di cui elimare l'albero dei processi (radice compresa)
void recursiveTermination(pcb_t* root);

// SYSCALL #3
// pid: riferimento al processo da eliminare
void terminateProcess(void *pid);

// SYSCALL #4
// semaddr: key del sameforo sul quale effettuare la Verhogen
void verhogen(int *semaddr);

// SYSCALL #5
// semaddr: key del sameforo sul quale effettuare la Passaren
void passeren(int *semaddr);

// SYSCALL #6
// command: comando da eseguire
// reg: puntatore all'indirizzo del device sul quale effettuare l'operazione
// subdevice: in caso di esecuzione su terminale, ne individua il sottodevice corretto
void waitIO(unsigned int command, unsigned int *reg, int subdevice);

// SYSCALL #7
// type: SYSCALL = 0, TLB = 1, TRAP = 2
// old_st: puntatore all'indirizzo da utilizzare come old area
// old_st: puntatore all'indirizzo da utilizzare come new area
void specPassup(int type, state_t *old_st, state_t *new_st);

// SYSCALL #8
// pid: identificatore del processo corrente
// ppid: identificatore del processo padre del corrente
void getPid(void **pid, void **ppid);

#endif