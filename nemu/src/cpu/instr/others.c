#include "cpu/instr.h"

make_instr_func(push_r_v){
	OPERAND r,dest;
	dest.data_size = r.data_size = data_size;
	if(data_size == 16)
		cpu.esp -= 2;
	if(data_size == 32)
		cpu.esp -= 4;

	r.type = OPR_REG;
	r.addr = opcode & 0x7;

	dest.type = OPR_MEM;
	dest.addr = cpu.esp;
	
	operand_read(&r);
	dest.val = r.val;
	operand_write(&dest);
	return 1;
}

make_instr_func(sub_i2rm_bv){
	OPERAND rm, imm;
	rm.data_size = data_size;
	imm.data_size = 8;

	int len = 1;
	len += modrm_rm(eip + 1,&rm);
	
	imm.type = OPR_IMM;
	imm.addr = eip + len;
	operand_read(&imm);
	uint32_t tval = sign_ext(imm.val, 8);
	rm.val = alu_sub(tval, rm.val, data_size);
	operand_write(&rm);
	return len + 1;
}

make_instr_func(cmp_i2rm_bv){
	OPERAND rm, imm;
	rm.data_size = data_size;
	imm.data_size = 8;

	int len = 1;
	len += modrm_rm(eip + 1,&rm);

	imm.type = OPR_IMM;
	imm.addr = eip + len;
	operand_read(&imm);
	uint32_t tval = sign_ext(imm.val, 8);
	alu_sub(tval, rm.val, data_size);
	return len + 1;
}

make_instr_func(call_near){
	uint32_t eipval = 0;
	OPERAND dest,addr;
	if(data_size == 16){
		cpu.esp -= 2;
		eipval = cpu.eip & 0xffff;
	}
	if(data_size == 32)
		cpu.esp -= 4;

	dest.data_size = data_size;
	dest.type = OPR_MEM;
	dest.addr = cpu.esp;
	dest.val = eipval;
	operand_write(&dest);

	addr.data_size = data_size;
	addr.type = OPR_IMM;
	operand_read(&addr);

	eipval += addr.val;
	if(data_size == 16){
		cpu.eip = eipval & 0xffff;	
	}
	if(data_size == 32){
		cpu.eip = eipval;
	}
	return 1 + data_size / 8;
}
