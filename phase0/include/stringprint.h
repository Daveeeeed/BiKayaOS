#ifndef __STRINGPRINT_H__
#define __STRINGPRINT_H__

#include "terminal.h"

/* Ritorna il carattere inserito in term */
static char charPressed(termreg_t *term);

/* Trasmetter il carattere c al terminale numero x */
static int termPutChar(char c, int x);

/* Trasmette il carattere c alla stampante numero x */
static int printPutChar(char c, int x);

/* Copia il testo inserito nel terminale x nella stringa str fino ad una lunghezza massima max_len */
int saveString(char *str, int max_len, int x);

/* Stampa la stringa str nel terminale numero x */
void termPrint(char *str, int x);

/* Stampa la stringa str nella stampante numero x */
void printPrint(char *str, int x);

#endif
