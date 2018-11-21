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

	print_asm_1("push","v",1,&r);	
	operand_read(&r);
	dest.val = r.val;
	operand_write(&dest);
	return 1;
}

make_instr_func(pop_r_v){
	OPERAND r, top;
	r.data_size = top.data_size = data_size;
	
	r.type = OPR_REG;
	r.addr = opcode & 0x7;

	top.type = OPR_MEM;
	top.addr = cpu.esp;

	print_asm_1("pop","v",1,&r);

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
	r.val = tval;
	operand_write(&r);
	return 1;
}

make_instr_func(push_i_b){
	OPERAND imm, dest;
	imm.data_size = 8;
	dest.data_size = data_size;
	if(data_size == 16)
		cpu.esp -= 2;
	if(data_size == 32)
		cpu.esp -= 4;
	
	imm.type = OPR_IMM;
	imm.addr = eip + 1;
	
	dest.type = OPR_MEM;
	dest.addr = cpu.esp;


	operand_read(&imm);

	imm.val &= 0xff;
	print_asm_1("push","b",2,&imm);	

	int dval = sign_ext(imm.val, 8);
	dest.val = dval;
	operand_write(&dest);
	return 2;	
}


make_instr_func(push_i_v){
	OPERAND imm, dest;
	imm.sreg = SREG_CS;
	dest.sreg = SREG_SS;
	imm.data_size = dest.data_size = data_size;
	if(data_size == 16)
		cpu.esp -= 2;
	if(data_size == 32)
		cpu.esp -= 4;
	
	imm.type = OPR_IMM;
	imm.addr = eip + 1;
	
	dest.type = OPR_MEM;
	dest.addr = cpu.esp;


	operand_read(&imm);

	print_asm_1("push","v",2,&imm);	

	int dval = sign_ext(imm.val, data_size);
	dest.val = dval;
	operand_write(&dest);
	return 1 + data_size / 8;	
}
make_instr_func(push_rm_v){
	OPERAND rm, dest;
	rm.sreg = SREG_CS;
	dest.sreg = SREG_SS;
	dest.data_size = rm.data_size = data_size;
	if(data_size == 16)
		cpu.esp -= 2;
	if(data_size == 32)
		cpu.esp -= 4;

	rm.type = OPR_MEM;
	
	int len = 1;
	len += modrm_rm(eip + 1, &rm);

	dest.type = OPR_MEM;
	dest.addr = cpu.esp;


	operand_read(&rm);
	print_asm_1("push","v",len,&rm);	
	if(data_size == 16)
		dest.val = rm.val & 0xffff;
	if(data_size == 32)
		dest.val = rm.val;
	operand_write(&dest);
	return len;
}

make_instr_func(pusha){
	OPERAND dest;
	dest.data_size = data_size;
	dest.type = OPR_MEM;
	dest.sreg = SREG_SS;

	if(data_size == 16){
		uint16_t tmp = cpu.esp;
		for(int i = 0; i < 8; i++){
			cpu.esp -= 2;
			dest.addr = cpu.esp;
			if(i == 4){
				dest.val = tmp;
			}
			else{
				dest.val = cpu.gpr[i]._16;
			}
			operand_write(&dest);
		}
	}
	else if(data_size == 32){
		uint32_t tmp = cpu.esp;
		for(int i = 0; i < 8; i++){
			cpu.esp -= 4;
			dest.addr = cpu.esp;
			if(i == 4){
				dest.val = tmp;
			}
			else{
				dest.val = cpu.gpr[i]._32;
			}
			operand_write(&dest);
		}	
	}
	print_asm_0("push","a",1);
	return 1;
}

make_instr_func(popa){
	OPERAND src;
	src.type = OPR_MEM;
	src.data_size = data_size;

	if(data_size == 16){
		for(int i = 0; i < 8; i++){
			if(i != 4){
				src.addr = cpu.esp;
				operand_read(&src);
				cpu.gpr[i]._16 = src.val;
				cpu.esp += 2;
			}
		}		
	}
	if(data_size == 16){
		for(int i = 0; i < 8;i++){
			if(i != 4){
				src.addr = cpu.esp;
				operand_read(&src);
				cpu.gpr[i]._32 = src.val;
				cpu.esp += 4;
			}
		}
	}
	print_asm_0("pop","a",1);

	return 1;
}
