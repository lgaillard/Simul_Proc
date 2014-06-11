//Programme qui teste l'instruction LOAD

TEXT
start   EQU	*

LOAD    R00, @op1
LOAD    R01, @0x2 
ADD	R00, #3
NOP
SUB	R00, #3
NOP
NOP
LOAD 	R02, @4
LOAD 	R03, #5
LOAD	R04, #op4
LOAD	R05, op1[R04] //On doit obtenir 7
LOAD	R06, +2[R04] //On doit obtenir 8
LOAD	R07, -2[R04] //On doit obtenir 4

NOP
HALT        

	END

//-----------------
// Données et pile
//-----------------
DATA 
        
        WORD 0
op1     WORD 1
op2     WORD 2
	WORD 0
op3	WORD 4
	WORD 0
op4	WORD 6       
	WORD 7
	WORD 8
        END
