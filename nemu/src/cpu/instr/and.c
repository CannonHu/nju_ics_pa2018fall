#include "cpu/instr.h"

static void instr_execute_2op(){
	operand_read(&opr_src);
	operand_read(&opr_dest);
	opr_dest.val = alu_and(opr_src.val, opr_dest.val, data_size);
	operand_write(&opr_dest);
}

make_instr_impl_2op(and, r, rm, v);
make_instr_impl_2op(and, rm, r, b);
make_instr_impl_2op(and, i, rm ,b);
make_instr_impl_2op(and, i, rm, v);

make_instr_func(and_i2rm_bv){
	OPERAND rm, imm;
	rm.data_size = imm.data_size = data_size;

	int len = 1;
	len += modrm_rm(eip + 1, &rm);
	
	imm.type = OPR_IMM;
	imm.addr = eip + len;

	operand_read(&imm);
	operand_read(&rm);
	int ival = sign_ext(imm.val, 8);
	rm.val = alu_and(ival, rm.val, data_size);
	operand_write(&rm);
	return 1 + len;
}
