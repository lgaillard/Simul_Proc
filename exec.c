#include <stdio.h>
#include "machine.h"
#include "instruction.h"
#include "error.h"


//! Décodage et exécution d'une instruction
/*!
 * \param pmach la machine/programme en cours d'exécution
 * \param instr l'instruction à exécuter
 * \return faux après l'exécution de \c HALT ; vrai sinon
 */
bool decode_execute(Machine *pmach, Instruction instr) {
Condition cond;
	switch (instr.instr_generic._cop) {
	case ILLOP : 
		error(ERR_ILLEGAL,pmach->_pc);
		return 1;
	
	case NOP : 
		return 1;
	
	case LOAD :
		if (instr.instr_generic._immediate) 
			pmach->_registers[instr.instr_generic._regcond]=instr.instr_immediate._value;
		else if (instr.instr_indexed._indexed)
			pmach->_registers[instr.instr_generic._regcond]=pmach->_data[pmach->_registers[instr.instr_indexed._rindex]+instr.instr_indexed._offset];
		else
			pmach->_registers[instr.instr_generic._regcond]=pmach->_data[instr.instr_absolute._address];
			
		if (pmach->_registers[instr.instr_generic._regcond] > 0)
			pmach->_cc = CC_P;
		else if (pmach->_registers[instr.instr_generic._regcond] == 0)
			pmach->_cc = CC_Z;
		else if (pmach->_registers[instr.instr_generic._regcond] < 0)
			pmach->_cc = CC_N;
		else 
			pmach->_cc = CC_U;
		return 1;
	
	case STORE : 
		if (instr.instr_generic._immediate) 
			error(ERR_IMMEDIATE,pmach->_pc);
		else if (instr.instr_generic._indexed)
			pmach->_data[pmach->_registers[instr.instr_indexed._rindex]+instr.instr_indexed._offset]=pmach->_registers[instr.instr_generic._regcond];
		else
			pmach->_data[instr.instr_absolute._address]=pmach->_registers[instr.instr_generic._regcond];
		return 1;
	
	case ADD :
		if (instr.instr_generic._immediate) 
			pmach->_registers[instr.instr_generic._regcond]+=instr.instr_immediate._value;
		else if (instr.instr_indexed._indexed)
			pmach->_registers[instr.instr_generic._regcond]+=pmach->_data[pmach->_registers[instr.instr_indexed._rindex]+instr.instr_indexed._offset];
		else
			pmach->_registers[instr.instr_generic._regcond]+=pmach->_data[instr.instr_absolute._address];
		
		if (pmach->_registers[instr.instr_generic._regcond] > 0)
			pmach->_cc = CC_P;
		else if (pmach->_registers[instr.instr_generic._regcond] == 0)
			pmach->_cc = CC_Z;
		else if (pmach->_registers[instr.instr_generic._regcond] < 0)
			pmach->_cc = CC_N;
		else 
			pmach->_cc = CC_U;
		return 1;
		
	case SUB : 
		if (instr.instr_generic._immediate) 
			pmach->_registers[instr.instr_generic._regcond]-=instr.instr_immediate._value;
		else if (instr.instr_generic._indexed)
			pmach->_registers[instr.instr_generic._regcond]-=pmach->_data[pmach->_registers[instr.instr_indexed._rindex]+instr.instr_indexed._offset];
		else
			pmach->_registers[instr.instr_generic._regcond]-=pmach->_data[instr.instr_absolute._address];
		
		if (pmach->_registers[instr.instr_generic._regcond] > 0)
			pmach->_cc = CC_P;
		else if (pmach->_registers[instr.instr_generic._regcond] == 0)
			pmach->_cc = CC_Z;
		else if (pmach->_registers[instr.instr_generic._regcond] < 0)
			pmach->_cc = CC_N;
		else 
			pmach->_cc = CC_U;
		return 1;
		
	case BRANCH : 
		if (instr.instr_generic._immediate) 
			error(ERR_IMMEDIATE,pmach->_pc);
		
		cond = (Condition) instr.instr_generic._regcond;
		//Test degeu pour tester C
		if (cond == NC || ((cond == EQ || cond == GE ||cond == LE)&& pmach->_cc == CC_Z)
			|| (pmach->_cc == CC_P && (cond == NE || cond == GE ||cond == GT))
			|| (pmach->_cc == CC_N && (cond == NE || cond == LE ||cond == LT))) {

			if (instr.instr_generic._indexed)
				pmach->_pc=pmach->_registers[instr.instr_indexed._rindex]+instr.instr_indexed._offset;
			else
				pmach->_pc=instr.instr_absolute._address;
			return 1;
		}
		else
			error(ERR_CONDITION,pmach->_pc);
		
	case CALL :
		if (instr.instr_generic._immediate) 
			error(ERR_IMMEDIATE,pmach->_pc);
		
		cond = (Condition) instr.instr_generic._regcond;
		//Test degeu pour tester C
		if (cond == NC || ((cond == EQ || cond == GE ||cond == LE)&& pmach->_cc == CC_Z)
			|| (pmach->_cc == CC_P && (cond == NE || cond == GE ||cond == GT))
			|| (pmach->_cc == CC_N && (cond == NE || cond == LE ||cond == LT))) {
			pmach->_data[pmach->_sp]= pmach->_pc;
			pmach->_sp -= 1;
			if (instr.instr_generic._indexed)
				pmach->_pc=pmach->_registers[instr.instr_indexed._rindex]+instr.instr_indexed._offset;
			else
				pmach->_pc=instr.instr_absolute._address;
			return 1;
		}
		else
			error(ERR_CONDITION,pmach->_pc);
	
	case RET :
		 pmach->_sp ++;
		 pmach->_pc = pmach->_data[pmach->_sp];
		 return 1;
	
	case PUSH : 
		if (instr.instr_generic._immediate) 
			pmach->_data[pmach->_sp] = instr.instr_immediate._value;
		else if (instr.instr_indexed._indexed)
			pmach->_data[pmach->_sp]=pmach->_data[pmach->_registers[instr.instr_indexed._rindex]+instr.instr_indexed._offset];
		else
			pmach->_data[pmach->_sp]=pmach->_data[instr.instr_absolute._address];
		pmach->_sp --;
		return 1;
	
	
	case POP:
		if (instr.instr_generic._immediate) 
			error(ERR_IMMEDIATE,pmach->_pc);
		pmach->_sp ++;
		
		if (instr.instr_indexed._indexed)
			pmach->_data[pmach->_registers[instr.instr_indexed._rindex]+instr.instr_indexed._offset]=pmach->_data[pmach->_sp];
		else
			pmach->_data[instr.instr_absolute._address]=pmach->_data[pmach->_sp];
		return 1;
		
	case HALT: //HALT
		warning(WARN_HALT,pmach->_pc);
		return 0;
		
	default :
		error(ERR_IMMEDIATE,pmach->_pc);
		return 0;
	}
}

//! Trace de l'exécution
/*!
 * On écrit l'adresse et l'instruction sous forme lisible.
 *
 * \param msg le message de trace
 * \param pmach la machine en cours d'exécution
 * \param instr l'instruction à exécuter
 * \param addr son adresse
 */
void trace(const char *msg, Machine *pmach, Instruction instr, unsigned addr){
	printf("TRACE: %s: 0x%04x: ", msg, addr);
	print_instruction(instr, addr);
	putchar('\n');
}

