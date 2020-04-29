#ifndef _INTERRUPT_H
#define _INTERRUPT_H

// Data una bitmap restituisce l'indice del device che ha sollevato un interrupt
int getDeviceNr(unsigned bitmap);

// Gestore di interrupt
void intHandler();

// DTNP handler
void dtpnHandler(int type);

// Terminal handler
void terminalHandler();

#endif

