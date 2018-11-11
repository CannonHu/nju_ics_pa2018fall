#include "cpu/instr.h"

static void instr_execute_2op(){
	operand_read(&opr_src);
	operand_read(&opr_dest);
	opr_dest.val = alu_or(opr_src.val, opr_dest.val, data_size);
	operand_write(&opr_dest);
}

make_instr_impl_2op(or, r, rm, b);
make_instr_impl_2op(or, r, rm, v);
make_instr_impl_2op(or, rm, r, b);
make_instr_impl_2op(or, rm, r, v);
make_instr_impl_2op(or, i, a, b);
make_instr_impl_2op(or, i, a, v);
make_instr_impl_2op(or, i, rm, b);
make_instr_impl_2op(or, i, rm, v);

make_instr_func(or_i2rm_bv){
	OPERAND rm, imm;
	rm.data_size = imm.data_size = data_size;

	int len = 1;
	len += modrm_rm(eip + 1, &rm);

	imm.type = OPR_IMM;
	imm.addr = eip + len;

	operand_read(&imm);
	operand_read(&rm);

	int iv = sign_ext(imm.val, 8);
	rm.val = alu_or(iv, rm.val, data_size);
	operand_write(&rm);
	
	print_asm_2("or","bv",len, &imm, &rm);

	return 1 + len;
}
