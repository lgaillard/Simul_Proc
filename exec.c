#include <stdio.h>
#include <stdbool.h>
#include <assert.h>
#include "machine.h"
#include "instruction.h"
#include "error.h"


unsigned int operand_address(Machine *pmach, Instruction instr) {
	assert(!instr.instr_generic._immediate);

	if (instr.instr_generic._indexed)
		return pmach->_registers[instr.instr_indexed._rindex] + instr.instr_indexed._offset;
	else
		return instr.instr_absolute._address;
}

void set_cc(Machine *pmach, unsigned int reg) {
	if (pmach->_registers[reg] > 0)
		pmach->_cc = CC_P;
	else if (pmach->_registers[reg] < 0)
		pmach->_cc = CC_N;
	else
		pmach->_cc = CC_Z;
}

bool check_condition(Machine *pmach, Condition cond) {

	if (pmach->_cc == CC_U && cond != NC)
		error(ERR_CONDITION, pmach->_pc);

	switch (cond) {
	case NC:
		return true;
	case EQ:
		return pmach->_cc == CC_Z;
	case NE:
		return pmach->_cc != CC_Z;
	case GT:
		return pmach->_cc == CC_P;
	case GE:
		return (pmach->_cc == CC_Z || pmach->_cc == CC_P);
	case LT:
		return pmach->_cc == CC_N;
	case LE:
		return (pmach->_cc == CC_Z || pmach->_cc == CC_N);
	default:
		assert(0);
	}
}

void exec_transfer(Machine *pmach, Instruction instr) {
	unsigned int oldpc = pmach->_pc - 1;

	int value;
	unsigned int op_address;
	unsigned reg = instr.instr_generic._regcond;

	if ((instr.instr_generic._cop == STORE || instr.instr_generic._cop == POP)
		&& instr.instr_generic._immediate)
		error(ERR_IMMEDIATE, pmach->_pc);
	
	if (instr.instr_generic._immediate) {
		value = instr.instr_immediate._value;
	}
	else {
		op_address = operand_address(pmach, instr);
		if (op_address >= pmach->_datasize)
			error(ERR_SEGDATA, oldpc);
		value = pmach->_data[op_address];
	}
	
	switch (instr.instr_generic._cop) {
	case LOAD:
		pmach->_registers[reg] = value;
		set_cc(pmach, reg);
		break;
	case STORE:
		pmach->_data[op_address] = pmach->_registers[reg];
		break;
	case ADD:
		pmach->_registers[reg] += value;
		set_cc(pmach, reg);
		break;
	case SUB:
		pmach->_registers[reg] -= value;
		set_cc(pmach, reg);
		break;
	case PUSH:
		pmach->_data[pmach->_sp--] = value;
		break;
	case POP:
		pmach->_data[op_address] = pmach->_data[++pmach->_sp];
		break;
	default:
		assert(0);
	}
}

void exec_branch(Machine *pmach, Instruction instr) {
	unsigned int oldpc = pmach->_pc - 1;

	unsigned int op_address = operand_address(pmach, instr);
	Condition cond = instr.instr_generic._regcond;

	if (instr.instr_generic._cop == BRANCH || instr.instr_generic._cop == CALL) {
		if (instr.instr_generic._immediate)
			error(ERR_IMMEDIATE, pmach->_pc);
		if (op_address >= pmach->_textsize)
			error(ERR_SEGTEXT, oldpc);
		if (cond > LAST_CONDITION)
			error(ERR_CONDITION, oldpc);

		if (check_condition(pmach, cond)) {
			if (instr.instr_generic._cop == CALL)
				pmach->_data[pmach->_sp--] = pmach->_pc;
			pmach->_pc = operand_address(pmach, instr);
		}
	} else if (instr.instr_generic._cop == RET) {
		pmach->_pc = pmach->_data[++pmach->_sp];
	} else {
		assert(0);
	}
}

//! Décodage et exécution d'une instruction
/*!
 * \param pmach la machine/programme en cours d'exécution
 * \param instr l'instruction à exécuter
 * \return faux après l'exécution de \c HALT ; vrai sinon
 */
bool decode_execute(Machine *pmach, Instruction instr) {
	unsigned int oldpc = pmach->_pc - 1;
	switch (instr.instr_generic._cop) {
	case ILLOP:
		error(ERR_ILLEGAL, oldpc);
	case HALT:
		warning(WARN_HALT, oldpc);
		return 0;
	case NOP:
		break;
	case LOAD:
	case STORE:
	case ADD:
	case SUB:
	case PUSH:
	case POP:
		exec_transfer(pmach, instr);
		break;
	case BRANCH:
	case CALL:
	case RET:
		exec_branch(pmach, instr);
		break;
	default:
		error(ERR_UNKNOWN, oldpc);
	}
	return true;
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

