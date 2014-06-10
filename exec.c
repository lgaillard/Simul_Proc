#include <stdio.h>
#include "machine.h"
#include "instruction.h"

//! Décodage et exécution d'une instruction
/*!
 * \param pmach la machine/programme en cours d'exécution
 * \param instr l'instruction à exécuter
 * \return faux après l'exécution de \c HALT ; vrai sinon
 */
bool decode_execute(Machine *pmach, Instruction instr) {
	switch (instr._cop) {
	case ILLOP : 
		error(ERR_ILLEGAL,instr.instr_absolute._address);
		return 1;
	
	case NOP : 
		return 1;
	
	case LOAD :
		if (instr._immediate) 
			pmach->_registers[instr._regcond]=insrt.instr_immediate._value;
		else if (instr._indexed)
			pmach->_registers[instr._regcond]=pmach->_data[pmach->_registers[instr.instr_indexed._rindex]+instr.instr_indexedt._offset];
		else
			pmach->_registers[instr._regcond]=pmach->_data[instr.instr_absolute._address];
		return 1;
	
	case STORE : 
		if (instr._immediate) 
			error(ERR_IMMEDIATE,instr.instr_absolute._address);
		else if (instr._indexed)
			pmach->_data[pmach->_registers[instr.instr_indexed._rindex]+instr.instr_indexedt._offset]=pmach->_registers[instr._regcond];
		else
			pmach->_data[instr.instr_absolute->_address]=pmach->_registers[instr._regcond];
		return 1;
	
	case ADD :
		if (instr._immediate) 
			pmach->_registers[instr._regcond]+=insrt.instr_immediate._value;
		else if (instr._indexed)
			pmach->_registers[instr._regcond]+=pmach->_data[pmach->_registers[instr.instr_indexed._rindex]+instr.instr_indexedt._offset];
		else
			pmach->_registers[instr._regcond]+=pmach->_data[instr.instr_absolute._address];
		
		if (pmach->_registers[instr._regcond] > 0)
			pmach->_cc = CC_P;
		else if (pmach->_registers[instr._regcond] == 0)
			pmach->_cc = CC_Z;
		else if (pmach->_registers[instr._regcond] < 0)
			pmach->_cc = CC_N;
		else 
			pmach->_cc = CC_U;
		return 1;
		
	case SUB : 
		if (instr._immediate) 
			pmach->_registers[instr._regcond]-=insrt.instr_immediate._value;
		else if (instr._indexed)
			pmach->_registers[instr._regcond]-=pmach->_data[pmach->_registers[instr.instr_indexed._rindex]+instr.instr_indexedt._offset];
		else
			pmach->_registers[instr._regcond]-=pmach->_data[instr.instr_absolute._address];
		
		if (pmach->_registers[instr._regcond] > 0)
			pmach->_cc = CC_P;
		else if (pmach->_registers[instr._regcond] == 0)
			pmach->_cc = CC_Z;
		else if (pmach->_registers[instr._regcond] < 0)
			pmach->_cc = CC_N;
		else 
			pmach->_cc = CC_U;
		return 1;
		
	case BRANCH : 
		if (instr._immediate) 
			error(ERR_IMMEDIATE,instr.instr_absolute._address);
		Condition cond = instr._regcond.condition;
		if (cond == NC || cond == GT) {
			else if (instr._indexed)
				pmach->_pc=pmach->pmach->_registers[instr.instr_indexed._rindex]+instr.instr_indexedt._offset;
			else
				pmach->_pc=instr.instr_absolute->_address;
			return 1;
		}
		else
			error(ERR_CONDITION,instr.instr_absolute._address);
		
	case CALL :
		if (instr._immediate) 
			error(ERR_IMMEDIATE,instr.instr_absolute._address);
		else if (instr._indexed)
			pmach->_data[pmach->_registers[instr.instr_indexed._rindex]+instr.instr_indexedt._offset]=pmach->_registers[instr._regcond];
		else
			pmach->_data[instr.instr_absolute->_address]=pmach->_registers[instr._regcond];
		return 1;
	
	case RET :
		 pmach->_sp ++;
		 pmach->_pc = pmach->_data[pmach->_sp];
		 return 1;
	
	case PUSH : 
		if (instr._immediate) 
			pmach->_data[pmach->sp] = insrt.instr_immediate._value;
		else if (instr._indexed)
			pmach->_data[pmach->sp]=pmach->_data[pmach->_registers[instr.instr_indexed._rindex]+instr.instr_indexedt._offset];
		else
			pmach->_data[pmach->sp]=pmach->_data[instr.instr_absolute->_address];
		pmach->_sp --;
		return 1;
	
	
	case POP:
		if (instr._immediate) 
			error(ERR_IMMEDIATE,instr.instr_absolute._address);
		pmach->_sp ++;
		else if (instr._indexed)
			pmach->_data[pmach->_registers[instr.instr_indexed._rindex]+instr.instr_indexedt._offset]=pmach->_data[pmach->sp];
		else
			pmach->_data[instr.instr_absolute->_address]=pmach->_data[pmach->sp];
		return 1;
		
	case HALT: //HALT
		warning(WARN_HALT,instr.instr_absolute._address);
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
	printf("TRACE: %s: ", msg);
	print_instruction(instr, addr);
	printf("\n");
}

