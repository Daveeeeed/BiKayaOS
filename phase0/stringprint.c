#include "stringprint.h"

static int charnotpressed(){

	unsigned int stat = getRecvdStat(term);

	if (stat != ST_READY && stat != ST_TRANS_RECV) return 0;

	term->recv_command = CMD_TRANS_RECV;

	while ((stat = getRecvdStat(term)) == ST_BUSY) ;

	tprint("FA");

	if (getRecvdChar(term) == "\n") {
		term->recv_command = CMD_ACK;
		return 0;
	}
	term->recv_command = CMD_ACK;
	return -1;
}

/* Salva l'input del terminal x sulla stringa str */
void saveString(char *str, int x){
	termreg_t *term0 = getTerm(x);
	int i = 0;
	
	while (charnotpressed()){
		int i;
		i = i+1;
		tprint("YYY");
	}
	tprint("cc\n");
}

void termPrint(char *str, int term_nr) {

}

void printPrint(char *str, int print_nr) {

}




