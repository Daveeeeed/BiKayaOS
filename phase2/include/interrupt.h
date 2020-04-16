#ifndef _INTERRUPT_H
#define _INTERRUPT_H

// DTNP handler
void diskHandler();
void tapeHandler();
void networkHandler();
void printerHandler();

// Terminal handler
void terminalHandler();

// Data una bitmap restituisce l'indice del device che ha sollevato un interrupt
int getDeviceNr(unsigned bitmap);

// Gestore di interrupt
void intHandler();

#endif

