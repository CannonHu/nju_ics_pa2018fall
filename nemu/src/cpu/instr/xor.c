#include "cpu/instr.h"

static void instr_execute_2op(){
	operand_read(&opr_src);
	operand_read(&opr_dest);
	printf("src: %x dest: %x",opr_src.val, opr_dest.val);
	opr_dest.val = alu_xor(opr_src.val, opr_dest.val, data_size);
	printf(" res: %x\n",opr_dest.val);
	operand_write(&opr_dest);
}

make_instr_impl_2op(xor, r, rm, v);
