//Programme qui teste l'instruction BRANCH

TEXT
start   EQU	*
	LOAD	R00, @0
	LOAD	R01, @1
	BRANCH 	NC, @label1
main1 	ADD R00, @0001
	BRANCH 	GE, @label2
main2	ADD R02, #1
	BRANCH LE, @label3 //Ne doit jamais prendre ce branchement
	ADD R04, #0
main3	BRANCH EQ, @label4 //prend ce branchement
main4 	ADD R04, #0
	BRANCH NE, @label5 //ne prend pas ce branchement
main5	ADD R04, #1
	BRANCH GT, @label6 //prend ce branchement
main6 	ADD R04, #0
	BRANCH LT, @label7 //ne prend pas ce branchement
main7	HALT



label1 	ADD R00, #2
	BRANCH	NC, @main1

label2	ADD R00, #7
	BRANCH NC, @main2

label3	SUB R00, #12
	BRANCH NC, @main3

label4	ADD R02, #1
	BRANCH NC,  @main4

label5	SUB R04, #1
	BRANCH NC, @main5

label6	SUB R04, #1
	BRANCH NC, @main6

label7	SUB R04, #1
	BRANCH NC, @main7
END

//-----------------
// Données et pile
//-----------------
DATA 
        
        WORD 10
	WORD 1
	
        END
