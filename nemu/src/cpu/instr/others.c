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
	src.typr = OPR_MEM;
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
	ret.val = eipval = eipval + 1 +data_size / 8;
	operand_write(&ret);

	dest.type = OPR_IMM;
	dest.addr = eip + 1;
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
	operand_read(&top);

	if(data_size == 16){
		cpu.eip = top.val & 0xffff;
		cpu.esp += 2;
	}
	if(data_size == 32){
		cpu.eip = top.val;
		cpu.esp += 4;
	}
	
	return 0;
}

make_instr_func(ret_near_imm16){
	OPERAND top, imm;
	top.type = OPR_MEM;
	imm.type = OPR_IMM;
	top.data_size = data_size;
	imm.data_size = 16;
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

make_instr_func(not_rm_v){
	OPERAND rm;
	rm.data_size = data_size;

	int len = 1;
	len += modrm_rm(eip + 1, &rm);

	operand_read(&rm);

	print_asm_1("not","",len, &rm);
	rm.val = ~rm.val;
	operand_write(&rm);
	return len;
}
