#include <stdio.h>
#include "debug.h"

#define ANSWSIZE 10

//! Affiche la liste des commandes disponibles en Debug
void help() {
	puts("Available commands:");
	puts("\th\thelp");
	puts("\tc\tcontinue (exit interactive debug mode)");
	puts("\ts\tstep by step (next instuction)");
	puts("\tRET\tstep by step (next instuction)");
	puts("\tr\tprint registers");
	puts("\td\tprint data memory");
	puts("\tt\tprint text (prograrm) memory");
	puts("\tp\tprint text (prograrm) memory");
	puts("\tm\tprint registers and data memory");
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
		fflush(stdout);
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