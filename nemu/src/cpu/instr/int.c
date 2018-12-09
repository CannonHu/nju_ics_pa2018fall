#include "cpu/instr.h"

make_instr_func(int_){
	OPERAND int_id;
	int_id.type = OPR_IMM;
	int_id.sreg = SREG_CS;
	int_id.data_size = 8;
	int_id.addr = eip + 1;

	operand_read(&int_id);

	uint8_t intr_no = int_id.val;
	raise_sw_intr(intr_no);
	print_asm_1("int", "", 2, &int_id);
	return 0;
}

make_instr_func(iret){
	OPERAND top;
	top.type = OPR_MEM;
	top.sreg = SREG_SS;
	top.data_size = 32;
	top.addr = cpu.esp;
	if(data_size == 32){
		
	}
	return 2;
}
