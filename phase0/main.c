#include "stringprint.h"

int main(void)
{	
	/* Crea stringa nella quale vengono salvati i caratteri scritti nel terminale */
	char[64] str;

	/* Salva l'input del terminale 0 sulla stringa str */
	saveString(str, 0);
	termPrint("STRINGA LETTA", 0);

	/* Stampa la stringa str sulla stampante 0 */
	printPrint("STRINGA LETTA:", 0);
	printPrint(str);

	/* Loop */
	while (1) WAIT();
	
	return 0;
}
