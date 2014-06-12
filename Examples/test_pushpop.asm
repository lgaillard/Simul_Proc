//Programme qui teste les instructions PUSH et POP


TEXT
start   EQU	*
        PUSH @op1
        PUSH @op2
	PUSH @op3
	PUSH @op4
        PUSH @op5
	NOP
	POP  @op5
        PUSH @op6
	HALT

	END

//-----------------
// Données et pile
//-----------------
DATA 
        
op1     WORD 1
op2	WORD 2
op3     WORD 3
op4	WORD 4
op5     WORD 5
op6	WORD 6
	
        END
