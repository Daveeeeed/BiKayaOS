#ifndef _SYSCALL_H
#define _SYSCALL_H

#include "types_bikaya.h"

void freeChildren(pcb_t *pid);

// Gestore system call
void sysHandler();

// SYSCALL #1
void getCpuTime(unsigned int *user, unsigned int *kernel, unsigned int *wallclock);

// SYSCALL #2
void createProcess(state_t *statep, int priority, void **cpid);

// SYSCALL #3
void terminateProcess(void *pid);

// SYSCALL #4
void verhogen(int *semaddr);

// SYSCALL #5
void passeren(int *semaddr);

// SYSCALL #6
void waitIO(unsigned int command, unsigned int *reg, int subdevice);

// SYSCALL #7
void specPassup(int type, state_t *old_st, state_t *new_st);

// SYSCALL #8
void getPid(void **pid, void **ppid);

#endif