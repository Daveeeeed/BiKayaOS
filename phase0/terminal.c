#include "terminal.h"

/* GET THE ADDRESS OF DESIRED TERMINAL */
termreg_t* getTerm(int x){
	if (x >= 0 && x <= 7)
	return (termreg_t *)DEV_REG_ADDR(IL_TERMINAL, x);
	else
	return NULL;
}

/* TRANSM_STATUS */
unsigned int getTransSChar(termreg_t *tp){
return tp->transm_status >> CHAR_OFFSET;
}

unsigned int getTransmStat(termreg_t *tp){
return tp->transm_status & TERM_STATUS_MASK;
}

/* RECV_STATUS */
unsigned int getRecvdChar(termreg_t *tp){
return tp->recv_status >> CHAR_OFFSET;
}

unsigned int getRecvdStat(termreg_t *tp){
return tp->recv_status & TERM_STATUS_MASK;
}

/* TRANSM_COMMAND */
unsigned int getTransCChar(termreg_t *tp){
return tp->transm_command >> CHAR_OFFSET;
}

unsigned int getTransmCmd(termreg_t *tp){
return tp->transm_command & TERM_STATUS_MASK;
}


