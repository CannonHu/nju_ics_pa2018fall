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
	cpu.esp -= 4;
	top.addr = esp;
	top.val = 
	
#endif
}

void raise_sw_intr(uint8_t intr_no) {
	// return address is the next instruction
	cpu.eip += 2;
	raise_intr(intr_no);
}
