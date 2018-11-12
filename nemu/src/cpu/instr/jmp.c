#include "cpu/instr.h"


make_instr_func(jmp_near) {
        OPERAND rel;
        rel.type = OPR_IMM;
	rel.sreg = SREG_CS;
        rel.data_size = data_size;
        rel.addr = eip + 1;

        operand_read(&rel);

	int offset = sign_ext(rel.val, data_size);
	print_asm_1("jmp", "", 2, &rel);

	cpu.eip += offset;

        return 1 + data_size / 8;
}

make_instr_func(jmp_near_indirect) {
        OPERAND rm;
        rm.data_size = data_size;
	
	int len = 1;
	len += modrm_rm(eip + 1, &rm);

        operand_read(&rm);

	print_asm_1("jmp", "", 2, &rm);

	int offset = sign_ext(rm.val, data_size);

	cpu.eip = offset;

	if(data_size == 16)
		cpu.eip &= 0xffff;

        return 0;
}

make_instr_func(jmp_short){
        OPERAND rel;
        rel.type = OPR_IMM;
	rel.sreg = SREG_CS;
        rel.data_size = data_size;
        rel.addr = eip + 1;

        operand_read(&rel);
	rel.val &= 0xff;
	int offset = sign_ext(rel.val, 8);
	print_asm_1("jmp", "", 2, &rel);

	cpu.eip += offset;

        return 2;
	

}

make_instr_func(jmp_far_imm){

	OPERAND ptr_sec, ptr_off;
	ptr_sec.type = ptr_off.type = OPR_IMM;
	ptr_sec.sreg = ptr_off.sreg = SREG_CS;
	ptr_sec.data_size = 16;
	ptr_off.data_size = data_size;

	ptr_sec.addr = eip + 5;
	operand_read(&ptr_sec);

	ptr_off.addr = eip + 1;
	operand_read(&ptr_off);

	if(cpu.cr0.pe == 0){
		cpu.segReg[SREG_CS].val = ptr_sec.val;
		cpu.eip = ptr_off.val;
		if(data_size == 16){
			cpu.eip &= 0xffff;
		}
	}
	else
	{
		uint32_t addr = cpu.gdtr.base + ((ptr_sec.val >> 3) & 0xffff) * 8;
		SegDesc* sdt = (SegDesc*)addr;
	
		assert(sdt->present == 1);

		printf("reached\n");

		uint32_t limit = (sdt->limit_15_0) | (sdt->limit_19_16 << 16);
		assert(limit >= ptr_off.val);

		load_sreg(SREG_CS);
		cpu.eip = cpu.segReg[SREG_CS].base + ptr_off.val;
		if(data_size == 16){
			cpu.eip &= 0xffff;
		}
	}
	
	printf("reached\n");
	return 1 + 2 + data_size / 8;
}
