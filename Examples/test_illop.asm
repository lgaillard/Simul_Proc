TEXT
start   EQU	*

LOAD    R00, @op1
LOAD    R01, @op2
ILLOP 
ADD	R00, #3

	END

//-----------------
// Données et pile
//-----------------
DATA 
        
        WORD 0
op1     WORD 5
op2     WORD 5
        
        END
