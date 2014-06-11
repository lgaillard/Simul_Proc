//Programme qui teste l'instruction CALL

TEXT
start   EQU	*
	LOAD	R00, @0
	LOAD	R01, @1
	CALL 	NC, @sub1
main1 	ADD R00, @0001
	CALL 	GE, @sub2
main2	ADD R02, #1
	CALL LE, @sub3 //Ne doit jamais prendre ce branchement
	ADD R04, #0
main3	CALL EQ, @sub4 //prend ce branchement
main4 	ADD R04, #0
	CALL NE, @sub5 //ne prend pas ce branchement
main5	ADD R04, #1
	CALL GT, @sub6 //prend ce branchement
main6 	ADD R04, #0
	CALL LT, @sub7 //ne prend pas ce branchement
main7	HALT
	NOP
	NOP



sub1 	EQU *
	ADD R00, #2
	RET

sub2	EQU *
	ADD R00, #7
	RET

sub3	EQU *
	SUB R00, #12
	RET

sub4	EQU *
	ADD R02, #1
	RET

sub5	EQU *
	SUB R04, #1
	RET

sub6	EQU *
	SUB R04, #1
	RET

sub7	EQU *
	SUB R04, #1
	RET
END

//-----------------
// Données et pile
//-----------------
DATA 
        
        WORD 10
	WORD 1
	
        END
