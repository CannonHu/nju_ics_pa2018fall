#include "cpu/instr.h"

/*static void instr_execute_2op(){
	operand_read(&opr_src);
	opr_dest = alu_sub(opr_src.val, opr_dest.val ,data_size);
	operand_write(&opr_dest);
}*/

make_instr_func(sub_i2rm_bv){
	OPERAND rm, imm;
	rm.data_size = imm.data_size = data_size;

	int len = 1;
	len += modrm_rm(eip + 1, &rm);

	imm.type = OPR_IMM;
	imm.addr = eip + len;

	operand_read(&imm);
	int iv = sign_ext(imm.val, 8);
	rm.val = alu_sub(iv, rm.val, data_size);
	operand_write(&rm);
	return 1 + len;

}
