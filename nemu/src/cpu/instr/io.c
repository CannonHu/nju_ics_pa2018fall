#include "cpu/instr.h"

make_instr_func(in_i2r_b){
	OPERAND imm, r;
	imm.data_size = r.data_size = 8;
	imm.sreg = SREG_CS;

	r.type = OPR_REG;
	r.addr = 0;
	imm.type = OPR_IMM;
	imm.addr = eip + 1;
	
	operand_read(&imm);
	r.val = pio_read(imm.val, 1);
	operand_write(&r);
	return 2;
}

make_instr_func(in_i2r_v){
	OPERAND imm, r;
	imm.data_size = 8;
	r.data_size = data_size;
	imm.sreg = SREG_CS;

	r.type = OPR_REG;
	r.addr = 0;
	imm.type = OPR_IMM;
	imm.addr = eip + 1;

}
