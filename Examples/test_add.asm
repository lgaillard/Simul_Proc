//Programme qui teste l'instruction ADD
//Soit on ajoute une valeur directement, soit on ajoute ce qui est contenu à une adresse

TEXT
start   EQU	*
ADD 	R00, #2
STORE	R00, @1
ADD	R01, @1
HALT        

	END

//-----------------
// Données et pile
//-----------------
DATA 
        
        WORD 0
	WORD 0
	
        END
