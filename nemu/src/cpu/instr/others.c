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

make_instr_func(pop_r_v){
	OPERAND r, top;
	r.data_size = top.data_size = data_size;
	
	r.type = OPR_REG;
	r.addr = opcode & 0x7;

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
	r.val = tval;
	operand_write(&r);
	return 1;
}

make_instr_func(popa){
	OPERAND top;
	top.data_size = data_size;
	
	top.type = OPR_MEM;
	top.addr = cpu.esp;
	
	int tval = 0;
	if(data_size == 16){
		for(int i = 0; i < 8; i++){
			if(i == 4)
				continue;
			operand_read(&top);
			cpu.esp += 2;
			cpu.grp[i]._16 = top.val & 0xffff;	
		}
	}
	if(data_size == 32){
		for(int i = 0; i < 8; i++){
			if(i == 4)
				continue;
			operand_read(&top);
			cpu.esp += 4;
			cpu.grp[i]._32 = top.val;	
		}
	
	}
	return 1;
}

make_instr_func(push_i_b){
	OPERAND imm, dest;
	dest.data_size = data_size;
	imm.data_size = 8;
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
	int dval = sign_ext(imm.val, 8);
	dest.val = dval;
	operand_write(&dest);
	return 2;	
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
	if(data_size == 16)
		dest.val = rm.val & 0xffff;
	if(data_size == 32)
		dest.val = rm.val;
	operand_write(&dest);
	return len;
}

make_instr_func(call_near){
	uint32_t eipval = 0;
	OPERAND dest,addr;

	if(data_size == 16){
		cpu.esp -= 2;
		eipval = cpu.eip & 0xffff;
	}
	if(data_size == 32){
		cpu.esp -= 4;
		eipval = cpu.eip;
	}

	dest.data_size = data_size;
	dest.type = OPR_MEM;
	dest.addr = cpu.esp;
	dest.val = eipval = eipval + 1 +data_size / 8;
	operand_write(&dest);

	addr.data_size = data_size;
	addr.type = OPR_IMM;
	addr.addr = eip + 1;
	operand_read(&addr);

	eipval += addr.val;

	if(data_size == 16){
		cpu.eip = eipval & 0xffff;	
	}
	if(data_size == 32){
		cpu.eip = eipval;
	}
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
	rm.val = ~rm.val;
	operand_write(&rm);
	return len;
}
