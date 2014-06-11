//Programme qui teste l'instruction BRANCH
//Erreur de fonctionnement ici chez nous : refuse d'exécuter jusqu'au HALt, soulève une forbidden value at addresse 0x5 (l'addresse de HALT)
//A  bien compris qu'il ne doit pas aller vers label2 cependant
TEXT
start   EQU	*
LOAD	R00, @0
LOAD	R01, @1
BRANCH 	NC, @label1
main1 	ADD R00, @0001
BRANCH LE, @label2 //Ne doit jamais prendre ce branchement
main2	HALT

label1 	ADD R00, #2
BRANCH	NC, @main1

label2	SUB R00, #12
BRANCH	NC, @main2

END

//-----------------
// Données et pile
//-----------------
DATA 
        
        WORD 10
	WORD 1
	
        END
