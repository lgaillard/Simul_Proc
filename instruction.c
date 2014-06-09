#include <stdio.h>
#include "error.h"
#include "instruction.h"

//! Forme imprimable des codes opérations
const char *cop_names[] = {
	"ILLOP",	//!< Instruction illégale
	"NOP",	//!< Instruction sans effet
	"LOAD",	//!< Chargement d'un registre
	"STORE",	//!< Rangement du contenu d'un registre 
	"ADD",	//!< Addition à un registre 
	"SUB",	//!< Soustraction d'un registre 
	"BRANCH",	//!< Branchement conditionnel ou non  
	"CALL",	//!< Appel de sous-programme 
	"RET",	//!< Retour de sous-programme 
	"PUSH",	//!< Empilement sur la pile d'exécution 
	"POP",	//!< Dépilement de la pile d'exécution
	"HALT",	//!< Arrêt (normal) du programme
};

//! Forme imprimable des conditions
const char *condition_names[] = {
	"NC", //!< Pas de condition (nrachement inconditionnel)
	"EQ",	//!< Égal à 0
	"NE",	//!< Différent de 0
	"GT", //!< Strictement positif
	"GE",	//!< Positif ou nul
	"LT", //!< Strictement négatif
	"LE", //!< Négatif ou null
};

//! Impression d'une instruction sous forme lisible (désassemblage)
/*!
* \param instr l'instruction à imprimer
* \param addr son adresse
*/
void print_instruction(Instruction instr, unsigned addr) {
	Code_Op cop = instr.instr_generic._cop;

	if (cop > LAST_COP)
		error(ERR_ILLEGAL, addr);

	printf("%s ", cop_names[cop]);

	if (cop == ILLOP || cop == NOP || cop == RET || cop == HALT)
		return;

	if (cop == BRANCH || cop == CALL) {
		if (instr.instr_generic._regcond > LAST_CONDITION)
			error(ERR_CONDITION, addr);

		printf("%s, ", condition_names[instr.instr_generic._regcond]);
	} else if(cop != PUSH && cop != POP) {
		printf("R%02u, ", instr.instr_generic._regcond);
	}

	if (instr.instr_generic._immediate) {
		printf("#%d", instr.instr_immediate._value);
	} else if (instr.instr_generic._indexed) {
		printf("%d[R%02u]", instr.instr_indexed._offset, instr.instr_indexed._rindex);
	}
	else {
		printf("@0x%04x", instr.instr_absolute._address);
	}
}
