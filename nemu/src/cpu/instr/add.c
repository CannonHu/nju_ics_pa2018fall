#include "cpu/instr.h"

static void instr_execute_2op(){
	operand_read(&opr_src);
	operand_read(&opr_dest);
	opr_dest.val = alu_add(opr_src.val, opr_dest.val, data_size);
	operand_write(&opr_dest);
}


make_instr_impl_2op(add, r, rm, b)
make_instr_impl_2op(add, r, rm, v)
make_instr_impl_2op(add, rm, r, b)
make_instr_impl_2op(add, rm, r, v)

make_instr_impl_2op(add, i, rm, b)
make_instr_impl_2op(add, i, rm, v)
make_instr_impl_2op(add, i, a, b)
make_instr_impl_2op(add, i, a, v)


make_instr_func(add_i2rm_bv){
	OPERAND rm, imm;
	rm.data_size = data_size;
	imm.data_size = 8;

	int len = 1;
	len += modrm_rm(eip + 1, &rm);

	imm.type = OPR_IMM;
	imm.addr = eip + len;
	imm.sreg = SREG_CS;

	operand_read(&imm);
	operand_read(&rm);
	print_asm_2("add","bv",len, &imm, &rm);
	int iv = imm.val;
	rm.val = alu_add(iv, rm.val, data_size);
	operand_write(&rm);
	return 1 + len;
}
