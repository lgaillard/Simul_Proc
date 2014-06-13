#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "error.h"

const char *error_names[] = {
    "No error",
    "Unknown instruction",
	"Illegal instruction",
    "Illegal condition",
    "Immediate value forbidden",
    "Segmentation fault in text",
    "Segmentation fault in data",
    "Segmentation fault in stack",
};

const char *warning_names[] = {
    "HALT reached",
};

//! Affichage d'une erreur et fin du simulateur
/*!
 * 
 * \param err code de l'erreur
 * \param addr adresse de l'erreur
 */
void error(Error err, unsigned addr){
    assert(err <= LAST_ERROR);
    fprintf(stderr, "ERROR: %s at address 0x%x\n", error_names[err], addr);
    exit(1);
}

//! Affichage d'un avertissement
/*!
 * \param warn code de l'avertissement
 * \param addr adresse de l'erreur
 */
void warning(Warning warn, unsigned addr){
    assert(warn <= LAST_WARNING);
    fprintf(stderr, "WARNING: %s at address 0x%x\n", warning_names[warn], addr);
}
