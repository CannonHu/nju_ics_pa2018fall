#include "cpu/instr.h"

make_instr_func(call_near){
	uint32_t eipval = 0;
	OPERAND dest,ret;

	if(data_size == 16){
		cpu.esp -= 2;
		eipval = cpu.eip & 0xffff;
	}
	if(data_size == 32){
		cpu.esp -= 4;
		eipval = cpu.eip;
	}

	ret.data_size = dest.data_size = data_size;
	ret.type = OPR_MEM;
	ret.addr = cpu.esp;
	ret.sreg = SREG_SS;
	ret.val = eipval = eipval + 1 +data_size / 8;
	operand_write(&ret);

	dest.type = OPR_IMM;
	dest.addr = eip + 1;
	dest.sreg = SREG_CS;
	operand_read(&dest);
	eipval += dest.val;

	if(data_size == 16){
		cpu.eip = eipval & 0xffff;	
	}
	if(data_size == 32){
		cpu.eip = eipval;
	}
	print_asm_0("call","",1);

	return 0;
}

make_instr_func(call_near_indirect){
	uint32_t eipval = 0;
	OPERAND dest, ret;
	if(data_size == 16){
		cpu.esp -= 2;
		eipval = cpu.eip & 0xffff;
	}
	if(data_size == 32){
		cpu.esp -= 4;
		eipval = cpu.eip;
	}

	ret.data_size = dest.data_size = data_size;
	ret.sreg = SREG_SS;
	dest.sreg = SREG_CS;
	
	int len = 1;
	len += modrm_rm(eip + 1, &dest);

	ret.type = OPR_MEM;
	ret.addr = cpu.esp;
	ret.val = eipval + len;
	operand_write(&ret);
	operand_read(&dest);

	if(data_size == 16){
		cpu.eip = dest.val & 0xffff;
	}
	if(data_size == 32){
		cpu.eip = dest.val;
	}

	print_asm_1("call","", len, &dest);

	return 0;

}

make_instr_func(ret_near){
	OPERAND top;
	top.type = OPR_MEM;
	top.data_size = data_size;
	top.addr = cpu.esp;
	top.sreg = SREG_SS;
	operand_read(&top);

	if(data_size == 16){
		cpu.eip = top.val & 0xffff;
		cpu.esp += 2;
	}
	if(data_size == 32){
		cpu.eip = top.val;
		cpu.esp += 4;
	}
	print_asm_0("ret","",1);
	return 0;
}

make_instr_func(ret_near_imm16){
	OPERAND top, imm;
	top.type = OPR_MEM;
	imm.type = OPR_IMM;
	top.data_size = data_size;
	imm.data_size = 16;
	top.sreg = SREG_SS;
	top.addr = cpu.esp;
	operand_read(&top);
	operand_read(&imm);

	if(data_size == 16){
		cpu.eip = top.val & 0xffff;
		cpu.esp += 2;
	}
	if(data_size == 32){
		cpu.eip = top.val;
		cpu.esp += 4;
	}
	
	cpu.esp += imm.val;
	
	return 0;
}


make_instr_func(lea){
	OPERAND r, rm;
	r.data_size = rm.data_size = data_size;

	int len = 1;
	len += modrm_r_rm(eip + 1, &r, &rm);
	print_asm_2("lea","",len,&rm, &r);	
	r.val = rm.addr;
	operand_write(&r);
	return len;

}

make_instr_func(leave){

	OPERAND ebp, top;
	ebp.data_size = top.data_size = data_size;
	ebp.sreg = top.sreg = SREG_SS;
	
	ebp.type = OPR_REG;
	ebp.addr = 0x5;
	operand_read(&ebp);

	cpu.esp = ebp.val;

	top.type = OPR_MEM;
	top.addr = cpu.esp;


	int tval = 0;
	operand_read(&top);
	if(data_size == 16){
		tval = top.val & 0xffff;
		cpu.esp += 2;
	}
	if(data_size == 32){
		tval = top.val;
		cpu.esp += 4;
	}
	ebp.val = tval;
	operand_write(&ebp);
	return 1;
}


make_instr_func(lgdt){

	int len = 1;

	OPERAND mem_addr;
	mem_addr.data_size = data_size;
	mem_addr.sreg = SREG_SS;
	len += modrm_rm(eip + 1, &mem_addr);
	operand_read(&mem_addr);


	OPERAND mem_lim, mem_base;
	mem_lim.data_size = 16;
	mem_lim.sreg = mem_base.sreg = SREG_SS;
	mem_lim.type = mem_base.type = OPR_MEM;
	if(data_size == 16){
		mem_base.data_size = 24;
	}
	else if(data_size == 32){
		mem_base.data_size = 32;
	
	}

	mem_lim.addr = mem_addr.val;
	operand_read(&mem_lim);
	cpu.gdtr.limit = mem_lim.val;

	mem_base.addr = mem_lim.addr + 2;
	operand_read(&mem_base);
	cpu.gdtr.base = mem_base.val;

	print_asm_1("lgdt","",2, &mem_addr);
	return len;
}

make_instr_func(lidt){

	int len = 2;

	OPERAND mem_addr;
	mem_addr.data_size = data_size;
	mem_addr.sreg = SREG_SS;
	len += modrm_rm(eip + 1, &mem_addr);
	operand_read(&mem_addr);

	OPERAND mem_lim, mem_base;
	mem_lim.data_size = 16;
	mem_lim.sreg = mem_base.sreg = SREG_SS;
	mem_lim.type = mem_base.type = OPR_MEM;
	if(data_size == 16){
		mem_base.data_size = 24;
	}
	else if(data_size == 32){
		mem_base.data_size = 32;
	}

	mem_lim.addr = mem_addr.val;
	operand_read(&mem_lim);
	cpu.idtr.limit = mem_lim.val;

	mem_base.addr = mem_lim.addr + 2;
	operand_read(&mem_base);
	cpu.idtr.base = mem_base.val;

	print_asm_0("lidt","",1);
	return len;
}
