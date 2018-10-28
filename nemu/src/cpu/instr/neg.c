#include "cpu/instr.h"

static void instr_execute_1op(){
	operand_read(&opr_src);
	opr_src.val = alu_sub(opr_src.val, 0, data_size);
	if(opr_src.val == 0){
		cpu.eflags.CF = 0;
	}
	else{
		cpu.eflags.CF = 1;
	}
	operand_write(&opr_src);
}

make_instr_impl_1op(not, rm, b);
make_instr_impl_1op(not, rm, v);
