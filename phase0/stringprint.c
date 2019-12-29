#include "stringprint.h"

static char charPressed(termreg_t *term){

	unsigned int stat = getRecvdStat(term);

	if (stat != ST_READY && stat != ST_TRANS_RECV) return '\n';

	term->recv_command = CMD_TRANS_RECV;

	while ((stat = getRecvdStat(term)) == ST_BUSY) ;

	char received = getRecvdChar(term);
	term->recv_command = CMD_ACK;
	return received;
}

static int termPutChar(char c, int x){
	
	termreg_t *term = getTerm(x);

	unsigned int stat = getTransmStat(term);

	if (stat != ST_READY && stat != ST_TRANS_RECV) return -1;

	setTransCmd(term, c);

	while ((stat = getTransmStat(term)) == ST_BUSY) ;

	term->transm_command = CMD_ACK;

    	if (stat != ST_TRANS_RECV)
		return -1;
    	else
		return 0;
}

static int printPutChar(char c, int x){
	
	dtpreg_t *printer = (dtpreg_t *)DEV_REG_ADDR(IL_PRINTER, x);

	unsigned int stat = printer->status;

	if (stat != ST_READY && stat != ST_TRANS_RECV) return -1;

	printer->data0 = c;
	printer->command = CMD_TRANS_RECV;

	while (printer->status == ST_BUSY) ;

	printer->command = CMD_ACK;

    	if (printer->status != ST_ERROR)
		return 0;
    	else
		return -1;
}



/* Salva l'input del terminal x sulla stringa str */
int saveString(char *str, int max_len, int x){

	termreg_t *term = getTerm(x);

	int i = 0;
	char input;

	while (i < max_len && (input = charPressed(term)) != '\n'){
		str[i] = input;
		i++;
	}
	return i;
}

void termPrint(char *str, int x) {
	while (*str)
		if (termPutChar(*str++, x))
			return;
}

void printPrint(char *str, int x) {
	while (*str)
		if (printPutChar(*str++, x))
			return;
}




