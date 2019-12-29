#ifndef __STRINGPRINT_H__
#define __STRINGPRINT_H__

#include "terminal.h"

static char charPressed(termreg_t *term);
static int termPutChar(char c, int x);
static int printPutChar(char c, int x);
int saveString(char *str, int max_len, int x);
void termPrint(char *str, int x);
void printPrint(char *str, int x);


#endif
