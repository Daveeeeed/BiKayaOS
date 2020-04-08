#ifndef _SYSCALL_H
#define _SYSCALL_H

#ifdef TARGET_UMPS

#endif

#ifdef TARGET_UARM

#endif

#define TERMINATEPROCESS 3

// Gestore System Call
void sys_handler();

// #3 System Call: termina il processo corrente
void terminate_process();

#endif