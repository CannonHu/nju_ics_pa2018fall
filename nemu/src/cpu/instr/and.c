#include "cpu/instr.h"


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
