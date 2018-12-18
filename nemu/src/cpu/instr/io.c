#include "cpu/instr.h"
#include "device/port_io.h"

make_instr_func(in_i2r_b){
	OPERAND imm, r;
	imm.data_size = r.data_size = 8;
	imm.sreg = SREG_CS;

	r.type = OPR_REG;
	r.addr = 0;
	imm.type = OPR_IMM;
	imm.addr = eip + 1;
	
	operand_read(&imm);
	r.val = pio_read(imm.val, 1) & 0xff;
	operand_write(&r);
	print_asm_2("in", "", 2, &imm, &r);
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

	operand_read(&imm);
	r.val = pio_read(imm.val, data_size/8);
	operand_write(&r);
	print_asm_2("in", "", 2, &imm, &r);
	return 2;
}

make_instr_func(out_r2i_b){
	OPERAND imm, r;
	imm.data_size = r.data_size = 8;
	imm.sreg = SREG_CS;

	r.type = OPR_REG;
	r.addr = 0;
	imm.type = OPR_IMM;
	imm.addr = eip + 1;

	operand_read(&imm);
	operand_read(&r);
	pio_write(imm.val, 1, r.val);
	print_asm_2("out", "", 2, &r, &imm);
	return 2;
}

make_instr_func(out_r2i_v){
	OPERAND imm, r;
	imm.data_size = 8;
	r.data_size = data_size;
	imm.sreg = SREG_CS;

	r.type = OPR_REG;
	r.addr = 0;
	imm.type = OPR_IMM;
	imm.addr = eip + 1;

	operand_read(&imm);
	operand_read(&r);
	pio_write(imm.val, data_size/8, r.val);
	print_asm_2("out", "", 2, &r, &imm);

	return 2;
}

make_instr_func(in_r2r_b){
	OPERAND rd, rp;
	rd.data_size = 8;
	rp.data_size = 16;

	rd.type = rp.type = OPR_REG;
	rd.addr = 0;
	rp.addr = 2;

	operand_read(&rp);
	rd.val = pio_read(rp.val, 1);
	operand_write(&rd);
	print_asm_2("in", "", 2, &rp, &rd);

	return 1;
}

make_instr_func(in_r2r_v){
	OPERAND rd, rp;
	rd.data_size = data_size;
	rp.data_size = 16;

	rd.type = rp.type = OPR_REG;
	rd.addr = 0;
	rp.addr = 2;

	operand_read(&rp);
	rd.val = pio_read(rp.val, data_size/8);
	operand_write(&rd);
	print_asm_2("in", "", 2, &rp, &rd);

	return 1;
}

make_instr_func(out_r2r_b){
	OPERAND rd, rp;
	rd.data_size = 8;
	rp.data_size = 16;

	rd.type = rp.type = OPR_REG;
	rd.addr = 0;
	rp.addr = 2;

	operand_read(&rp);
	operand_read(&rd);
	pio_write(rp.val, 1, rd.val);
	print_asm_2("out", "", 2, &rd, &rp);

	return 1;
}

make_instr_func(out_r2r_v){
	OPERAND rd, rp;
	rd.data_size = data_size;
	rp.data_size = 16;

	rd.type = rp.type = OPR_REG;
	rd.addr = 0;
	rp.addr = 2;

	operand_read(&rp);
	operand_read(&rd);
	pio_write(rp.val, data_size/8, rd.val);
	print_asm_2("out", "", 2, &rd, &rp);

	return 1;
}
