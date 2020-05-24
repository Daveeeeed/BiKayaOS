#ifndef _INTERRUPT_H
#define _INTERRUPT_H

// Data una bitmap restituisce l'indice del device (range 0-7) che ha sollevato un interrupt
int getDeviceNr(unsigned bitmap);

// Gestore iniziale degli interrupt
void intHandler();

// Gestore specifico degli interrupt sollevati da devices (escluso terminale)
void dtpHandler(int type);

// Gestore specifico degli interrupt sollevati da terminale
void terminalHandler();

#endif

