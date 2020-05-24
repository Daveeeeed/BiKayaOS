#ifndef _EXCEPTION_H
#define _EXCEPTION_H

// Gestore di Trap (attualmente non gestite fatta eccezione per la gestione tramite SPEC PASSUP)
void trapHandler();

// Gestore di TLB (attualmente non gestite fatta eccezione per la gestione tramite SPEC PASSUP)
void tlbHandler();

// Gestore delle eccezioni gestite tramite SPEC PASSUP
// type: tipo di eccezione da gestire (SYSCALL = 0, TLB = 1, TRAP = 2)
void specialHandler(unsigned type);

#endif