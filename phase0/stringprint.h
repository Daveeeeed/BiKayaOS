#ifndef __STRINGPRINT_H__
#define __STRINGPRINT_H__

#include "terminal.h"

static int charnotpressed();
void saveString(char *str));
void termPrint(char *str, int term_nr);
void printPrint(char *str, int print_nr);


#endif
