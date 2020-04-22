#ifndef _EXCEPTION_H
#define _EXCEPTION_H

// Gestori eccezioni non gestite
void trapHandler();
void tlbHandler();

// Gestore eccezioni speciali
void specHandler(unsigned type);

#endif