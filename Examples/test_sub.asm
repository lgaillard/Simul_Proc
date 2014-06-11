//Programme qui teste l'instruction SUB
//Programme qui teste l'instruction ADD
//Soit on ajoute une valeur directement, soit on ajoute ce qui est contenu à une adresse

TEXT
start   EQU	*
LOAD	R00, @0
LOAD	R01, @0
SUB 	R00, @1
SUB	R01, #5
HALT        

	END

//-----------------
// Données et pile
//-----------------
DATA 
        
        WORD 10
	WORD 1
	
        END
