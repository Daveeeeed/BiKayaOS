#ifndef _INTERRUPT_H
#define _INTERRUPT_H

// DTNP handler
void disk_handler();
void tape_handler();
void network_handler();
void printer_handler();

// Terminal handler
void terminal_handler();

// Data una bitmap restituisce l'indice del device che ha sollevato un interrupt
int get_device_nr(unsigned bitmap);

// Gestore di interrupt
void int_handler();

#endif

