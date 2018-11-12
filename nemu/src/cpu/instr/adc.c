#include "cpu/instr.h"

static void instr_execute_2op(){
	operand_read(&opr_src);
	operand_read(&opr_dest);
	opr_dest.val = alu_adc(opr_src.val, opr_dest.val, data_size);
	operand_write(&opr_dest);
}

make_instr_impl_2op(adc, r, rm, b);
make_instr_impl_2op(adc, r, rm, v);
make_instr_impl_2op(adc, rm, r, b);
make_instr_impl_2op(adc, rm, r, v);
make_instr_impl_2op(adc, i, a, b);
make_instr_impl_2op(adc, i, a, v);
make_instr_impl_2op(adc, i, rm, b);
make_instr_impl_2op(adc, i, rm, v);

make_instr_func(adc_i2rm_bv){
	OPERAND rm, imm;
	rm.data_size = imm.data_size = data_size;

	int len = 1;
	len += modrm_rm(eip + 1, &rm);

	imm.type = OPR_IMM;
	imm.addr = eip + len;
	imm.sreg = rm.sreg = SREG_CS;

	operand_read(&imm);
	operand_read(&rm);
	print_asm_2("adc","bv",len, &imm, &rm);
	int iv = sign_ext(imm.val, 8);
	rm.val = alu_adc(iv, rm.val, data_size);
	operand_write(&rm);
	return 1 + len;
}
