#include "machine.h"

//! Second exemple de segment de texte.
/*!
* \note On voit ici l'int�r�t de l'initialisation d'une union avec nommage
* explicite des champs.
*/

Instruction text[] = {
    //   type		 cop	imm	ind	regcond	operand
    //-------------------------------------------------------------
    { .instr_absolute = { PUSH, false, false, 0, 2 } },  // 0
    { .instr_absolute = { PUSH, false, false, 0, 3 } },  // 1
    { .instr_absolute = { CALL, false, false, NC, 10 } },  // 2
    { .instr_immediate = { ADD, true, false, 15, 2 } },  // 3 
    { .instr_absolute = { STORE, false, false, 0, 1 } },  // 4
    { .instr_generic = { POP, false, false, 0, 2 } },  // 5
    { .instr_generic = { POP, false, false, 0, 3 } },  // 6
    { .instr_generic = { HALT, } },  // 7
    { .instr_generic = { NOP, } },  // 8
    { .instr_generic = { NOP, } },  // 9
    { .instr_indexed = { LOAD, false, true, 0, 15, +3 } },  // 10
    { .instr_indexed = { LOAD, false, true, 1, 15, +2 } },  // 11
    { .instr_immediate = { SUB, true, false, 1, 1 } },  // 12
    { .instr_absolute = { BRANCH, false, false, LE, 17 } },  // 13
    { .instr_indexed = { ADD, false, true, 0, 15, +3 } },  // 14
    { .instr_immediate = { SUB, true, false, 1, 1 } },  // 15 
    { .instr_absolute = { BRANCH, false, false, NC, 13 } },  // 16  
    { .instr_generic = { RET, } },  // 17
};

//! Taille utile du programme
const unsigned textsize = sizeof(text) / sizeof(Instruction);

//! Premier exemple de segment de donn�es initial
Word data[20] = {
    0,  // 0
    0,  // 1: r�sultat
    20, // 2: premier op�rande
    5,  // 3: second op�rande
};

//! Fin de la zone de donn�es utile
const unsigned dataend = 10;

//! Taille utile du segment de donn�es
const unsigned datasize = sizeof(data) / sizeof(Word);