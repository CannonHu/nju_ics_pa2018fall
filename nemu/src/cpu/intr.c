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
	
	push sreg cs invisible part
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

	//push eip
	cpu.esp -= 4;
	top.addr = cpu.esp;
	top.data_size = 32;
	top.val = cpu.eip;
	operand_write(&top);
	
	//find IDT entry
	uint32_t idt_addr = cpu.idtr.base + intr_no * 8;
	GateDesc cur_gd;
	cur_gd.val[0] = paddr_read(idt_addr, 4);
	cur_gd.val[1] = paddr_read(idt_addr + 32, 4);

	assert(cur_gd.present == 1);
	if(cur_gd.type == 0xe){
		cpu.eflags.IF = 0;
	}
	
	cpu.cs.val = cur_gd.selector;
	cpu.eip = cur_gd.offset_15_0 + (cur_gd.offset_31_16 << 16);

#endif
}

void raise_sw_intr(uint8_t intr_no) {
	// return address is the next instruction
	cpu.eip += 2;
	raise_intr(intr_no);
}
