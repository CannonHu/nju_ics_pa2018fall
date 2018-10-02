#include "cpu/instr.h"

static void instr_execute_2op(){
	operand_read(&opr_src);
	opr_dest.val = opr_src.val + 1;
	operand_write(&opr_dest);
}

make_instr_impl_2op(inc, i, rm, v)
