#ifndef SYSCALL_H
#define SYSCALL_H

#define TERMINATEPROCESS 3

// Gestore System Call
void sys_handler();

// #3 System Call: termina il processo corrente
void terminate_process();

#endif