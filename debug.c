#include <stdio.h>
#include "debug.h"

#define ANSWSIZE 32767

//! Affiche la liste des commandes disponibles en Debug
void help() {
	printf("Available commands:\n");
	printf("\th\thelp\n");
	printf("\tc\tcontinue (exit interactive debug mode)\n");
	printf("\ts\tstep by step (next instuction)\n");
	printf("\tRET\tstep by step (next instuction)\n");
	printf("\tr\tprint registers\n");
	printf("\td\tprint data memory\n");
	printf("\tt\tprint text (prograrm) memory\n");
	printf("\tp\tprint text (prograrm) memory\n");
	printf("\tm\tprint registers and data memory\n");
}

//! Dialogue de mise au point interactive pour l'instruction courante.
/*!
* Cette fonction gère le dialogue pour l'option \c -d (debug). Dans ce mode,
* elle est invoquée après l'exécution de chaque instruction.  Elle affiche le
* menu de mise au point et on exécute le choix de l'utilisateur. Si cette
* fonction retourne faux, on abandonne le mode de mise au point interactive
* pour les instructions suivantes et jusqu'à la fin du programme.
*
* \param mach la machine/programme en cours de simulation
* \return vrai si l'on doit continuer en mode debug, faux sinon
*/
bool debug_ask(Machine *pmach) {
	char answer[ANSWSIZE];
	while (true) {
		printf("DEBUG? ");
		fflush(stdin);
		fgets(answer, ANSWSIZE, stdin);
		switch (answer[0]) {
		case 'h':
			help();
			break;
		case 'c':
			return false;
		case 's':
		case '\n':
			return true;
		case 'r':
			print_cpu(pmach);
			break;
		case 't':
		case 'p':
			print_program(pmach);
			break;
		case 'm':
			print_cpu(pmach);
		case 'd':
			print_data(pmach);
			break;
		}
	}
}