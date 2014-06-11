//Programme qui teste l'instruction STORE

TEXT
start   EQU	*
ADD 	R00, #2
STORE 	R00, @2
ADD	R01, #3
HALT        

	END

//-----------------
// Données et pile
//-----------------
DATA 
        
        WORD 0
	WORD 0
	
        END
