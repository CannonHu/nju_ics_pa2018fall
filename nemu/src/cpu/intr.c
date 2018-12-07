#include "cpu/intr.h"
#include "cpu/instr.h"
#include "memory/memory.h"


void raise_intr(uint8_t intr_no) {
#ifdef IA32_INTR/*
	printf("Please implement raise_intr()");
	assert(0);*/
	OPERAND top;
	top.type = OPR_MEM;
	top.sreg = SREG_SS;
	top.data_size = 32;

	//push eflags
	cpu.esp -= 4;
	top.addr = cpu.esp;
	top.val = cpu.eflags.val;
        operand_write(&top);

	//push sreg cs visible part
	cpu.esp -= 2;
	top.data_size = 16;
	top.addr = cpu.esp;
	top.val = cpu.cs.val;
	operand_write(&top);
	
	//push sreg cs invisible part
	cpu.esp -= 4;
	top.data_size = 32;
	top.addr = cpu.esp;
	top.val = cpu.cs.base;
	operand_write(&top);

	cpu.esp -= 4;
	top.addr = cpu.esp;
	top.val = cpu.cs.limit;
	operand_write(&top);

	cpu.esp -= 1;
	top.data_size = 8;
	top.addr = cpu.esp;
	top.val = cpu.cs.type + cpu.cs.privilege_level << 5 + cpu.cs.soft_use << 7;
	operand_write(&top);

	cpu.esp -= 4;
	top.addr = cpu.esp;
	top.data_size = 32;
	top.val = cpu.eip;
	operand_write(&top);
	
	
#endif
}

void raise_sw_intr(uint8_t intr_no) {
	// return address is the next instruction
	cpu.eip += 2;
	raise_intr(intr_no);
}
