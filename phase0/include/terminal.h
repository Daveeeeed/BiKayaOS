#ifndef __TERMINAL_H__
#define __TERMINAL_H__

#include "system.h"

#define ST_NOT_INSTALLED   0
#define ST_READY           1
#define ST_ILLEGAL_OP      2
#define ST_BUSY            3
#define ST_ERROR           4
#define ST_TRANS_RECV      5

#define CMD_RESET          0
#define CMD_ACK            1
#define CMD_TRANS_RECV     2

#define CHAR_OFFSET        8
#define TERM_STATUS_MASK   0xFF


/* GET THE ADDRESS OF DESIRED TERMINAL */
termreg_t* getTerm(int x);

/* TRANSM_STATUS */
unsigned int getTransSChar(termreg_t *tp);
unsigned int getTransmStat(termreg_t *tp);

/* RECV_STATUS */
unsigned int getRecvdChar(termreg_t *tp);
unsigned int getRecvdStat(termreg_t *tp);

/* TRANSM_COMMAND */
unsigned int getTransCChar(termreg_t *tp);
unsigned int getTransmCmd(termreg_t *tp);
void setTransCmd(termreg_t *tp, char c);

#endif
