#include "cpu/instr.h"

make_instr_func(cmp_i2rm_bv){
	OPERAND rm, imm;
	rm.data_size = data_size;
	imm.data_size = 8;

	int len = 1;
	len += modrm_rm(eip + 1, &rm);

	imm.type = OPR_IMM;
	imm.addr = eip + len;
	operand_read(&imm);
	operand_read(&rm);

	uint32_t tval = sign_ext(imm.val, 8);
	alu_sub(tval, rm.val, data_size);
	return 1 + len;
}
