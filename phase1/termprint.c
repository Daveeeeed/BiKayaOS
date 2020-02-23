#include "system.h"

#define ST_READY           1
#define ST_BUSY            3
#define ST_TRANSMITTED     5

#define CMD_ACK            1
#define CMD_TRANSMIT       2

#define CHAR_OFFSET        8
#define TERM_STATUS_MASK   0xFF


static termreg_t *term0_reg = (termreg_t *) DEV_REG_ADDR(IL_TERMINAL, 0);
static dtpreg_t *print0_reg = (dtpreg_t *) DEV_REG_ADDR(IL_PRINTER, 0);

static unsigned int px_status(dtpreg_t *pr)
{
    return (pr->status);
}

static unsigned int tx_status(termreg_t *tp)
{
    return ((tp->transm_status) & TERM_STATUS_MASK); //mask that leaves only the least significant byte of transm_status
}

static int term_putchar(char c)
{
    unsigned int stat;
    stat = tx_status(term0_reg);
    if (stat != ST_READY && stat != ST_TRANSMITTED)
        return -1;

    print0_reg->data0 = c;
    print0_reg->command = 2;
    while (print0_reg->status == ST_BUSY)
        ;

    print0_reg->command = CMD_ACK;

    if (print0_reg->status != 1)
        return -1;

    term0_reg->transm_command = ((c << CHAR_OFFSET) | CMD_TRANSMIT); //shifta i bit di c di CHAR_OFFSET posizioni (li mette nel campo transmit char e aggiunge il comando)

    while ((stat = tx_status(term0_reg)) == ST_BUSY)
        ;
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                  
    term0_reg->transm_command = CMD_ACK;

    if (stat != ST_TRANSMITTED)
        return -1;
    else
        return 0;
    
}

//stampare sulla stampante una stringa letta dal terminale

void term_puts(const char *str)
{
    while (*str)
        if (term_putchar(*str++))
            return;
}
