#include "stringprint.h"

#define MAX_LEN 64

int main(void)
{	
	/* Crea stringa nella quale vengono salvati i caratteri scritti nel terminale */
	char str[MAX_LEN];

	/* Salva l'input del terminale 0 sulla stringa str */
	int i = saveString(str, MAX_LEN, 0);
	str[i++] = '\0';
	termPrint("STRINGA LETTA!\n", 0);
	/* Stampa la stringa str sulla stampante 0 */
	printPrint("STRINGA LETTA: ", 0);
	printPrint(str, 0);

	/* Loop */
	while (1) WAIT();
	
	return 0;
}
