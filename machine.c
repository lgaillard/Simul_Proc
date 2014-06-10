#include <stdio.h>
#include "machine.h"
#include "error.h"
#include "debug.h"
#include "exec.h"

const char cc_names[] = {
    'U',
    'Z',
    'P',
    'N',
};

void config_error(const char *programfile, const char *error) {
    fprintf(stderr, "Bad program file %s: %s\n", programfile, error);
    exit(1);
}

//! Chargement d'un programme
/*!
* La machine est r�initialis�e et ses segments de texte et de donn�es sont
* remplac�s par ceux fournis en param�tre.
*
* \param pmach la machine en cours d'ex�cution
* \param textsize taille utile du segment de texte
* \param text le contenu du segment de texte
* \param datasize taille utile du segment de donn�es
* \param data le contenu initial du segment de texte
*/
void load_program(Machine *pmach,
    unsigned textsize, Instruction text[textsize],
    unsigned datasize, Word data[datasize], unsigned dataend) {

    pmach->_text = text;
    pmach->_textsize = textsize;
    pmach->_data = data;
    pmach->_datasize = datasize;
    pmach->_dataend = dataend;

    pmach->_pc = 0;
    pmach->_cc = CC_U;

    for (int i = 0; i < (NREGISTERS - 1); ++i) {
        pmach->_registers[i] = 0;
    }
    pmach->_sp = datasize - 1;
}

//! Lecture d'un programme depuis un fichier binaire
/*!
* Le fichier binaire a le format suivant :
*
*    - 3 entiers non sign�s, la taille du segment de texte (\c textsize),
*    celle du segment de donn�es (\c datasize) et la premi�re adresse libre de
*    donn�es (\c dataend) ;
*
*    - une suite de \c textsize entiers non sign�s repr�sentant le contenu du
*    segment de texte (les instructions) ;
*
*    - une suite de \c datasize entiers non sign�s repr�sentant le contenu initial du
*    segment de donn�es.
*
* Tous les entiers font 32 bits et les adresses de chaque segment commencent �
* 0. La fonction initialise compl�tement la machine.
*
* \param pmach la machine � simuler
* \param programfile le nom du fichier binaire
*
*/
void read_program(Machine *mach, const char *programfile) {
    FILE *file;
    if (!(file = fopen(programfile, "r")))
        config_error(programfile, "Cannot open program file");

    unsigned sizes[3];
    if (fread(sizes, sizeof(unsigned), 3, file) < 3)
        config_error(programfile, "Incorrect segment dimensions");

    Instruction *text = malloc(sizes[0] * sizeof(Instruction));
    if (fread(text, sizeof(Instruction), sizes[0], file) < sizes[0])
        config_error(programfile, "Too many instructions");

    Word *data = malloc(sizes[1] * sizeof(Word));
    if (fread(data, sizeof(Word), sizes[1], file) < sizes[1])
        config_error(programfile, "Too many data");

    fclose(file);

    if (sizes[2] > sizes[1])
        config_error(programfile, "Data lenght greater than memory size");
    if ((sizes[1] - sizes[2]) < MINSTACKSIZE)
        config_error(programfile, "Not enough room for stack");

    load_program(mach, sizes[0], text, sizes[1], data, sizes[2]);
}

//! Affichage du programme et des donn�es
/*!
* On affiche les instruction et les donn�es en format hexad�cimal, sous une
* forme pr�te � �tre coup�e-coll�e dans le simulateur.
*
* Pendant qu'on y est, on produit aussi un dump binaire dans le fichier
* dump.prog. Le format de ce fichier est compatible avec l'option -b de
* test_simul.
*
* \param pmach la machine en cours d'ex�cution
*/
void dump_memory(Machine *pmach) {
    // Print on screen
    printf("Instruction text[] = {");
    for (int i = 0; i < pmach->_textsize; ++i) {
        if (i % 4 == 0) printf("\n    ");
        printf("0x%08x, ", pmach->_text[i]._raw);
    }
    printf("\n};\nunsigned textsize = %u;\n\n", pmach->_textsize);
    printf("Word data[] = {");
    for (int i = 0; i < pmach->_datasize; ++i) {
        if (i % 4 == 0) printf("\n    ");
        printf("0x%08x, ", pmach->_data[i]);
    }
    printf("\n};\nunsigned datasize = %u;\nunsigned dataend = %u;\n", pmach->_datasize, pmach->_dataend);

    // Print on file
    FILE *file;
    if ((file = fopen("dump.bin", "w"))) {
        fwrite(&pmach->_textsize, sizeof(unsigned), 1, file);
        fwrite(&pmach->_datasize, sizeof(unsigned), 1, file);
        fwrite(&pmach->_dataend, sizeof(unsigned), 1, file);
        fwrite(&pmach->_text->_raw, sizeof(Word), pmach->_textsize, file);
        fwrite(pmach->_data, sizeof(Word), pmach->_datasize, file);
        fclose(file);
    }
}

//! Affichage des instructions du programme
/*!
* Les instructions sont affich�es sous forme symbolique, pr�c�d�es de leur adresse.
.*
* \param pmach la machine en cours d'ex�cution
*/
void print_program(Machine *pmach) {
    printf("\n*** PROGRAM (size: %u) ***\n", pmach->_textsize);
    for (int i = 0; i < pmach->_textsize; ++i) {
        printf("0x%04x: 0x%08x \t ", i, pmach->_text[i]._raw);
        print_instruction(pmach->_text[i], i);
        putchar('\n');
    }
    putchar('\n');
}

//! Affichage des donn�es du programme
/*!
* Les valeurs sont affich�es en format hexad�cimal et d�cimal.
*
* \param pmach la machine en cours d'ex�cution
*/
void print_data(Machine *pmach) {
    printf("*** DATA (size: %u, end = 0x%08x (%u)) ***", pmach->_datasize, pmach->_dataend, pmach->_dataend);
    for (int i = 0; i < pmach->_datasize; ++i) {
        if (i % 3 == 0) printf("\n");
        printf("0x%04x: 0x%08x %-4d   ", i, pmach->_data[i], pmach->_data[i]);
    }
    printf("\n\n");
}

//! Affichage des registres du CPU
/*!
* Les registres g�n�raux sont affich�es en format hexad�cimal et d�cimal.
*
* \param pmach la machine en cours d'ex�cution
*/
void print_cpu(Machine *pmach) {
    printf("\n*** CPU ***\n");
    printf("PC:  0x%08x   CC: %c\n", pmach->_pc, cc_names[pmach->_cc]);
    for (int i = 0; i < NREGISTERS; ++i) {
        if (i % 3 == 0) printf("\n");
        printf("R%02u: 0x%08x %-4d   ", i, pmach->_registers[i], pmach->_registers[i]);
    }
    printf("\n\n");
}

//! Simulation
/*!
* La boucle de simualtion est tr�s simple : recherche de l'instruction
* suivante (point�e par le compteur ordinal \c _pc) puis d�codage et ex�cution
* de l'instruction.
*
* \param pmach la machine en cours d'ex�cution
* \param debug mode de mise au point (pas � apas) ?
*/
void simul(Machine *pmach, bool debug) {
    bool running = true;
    while (running) {
        if (pmach->_pc >= pmach->_textsize)
            error(ERR_SEGTEXT, pmach->_pc);

        trace("Executing", pmach, pmach->_text[pmach->_pc], pmach->_pc);

        running = decode_execute(pmach, pmach->_text[pmach->_pc++]);

        if (debug)
            debug = debug_ask(pmach);
    }
}