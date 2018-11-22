#include "cpu/cpu.h"
#include "memory/memory.h"

// return the linear address from the virtual address and segment selector
uint32_t segment_translate(uint32_t offset, uint8_t sreg) {
	/* TODO: perform segment translation from virtual address to linear address
	 * by reading the invisible part of the segment register 'sreg'
	 */

	/*if(sreg == SREG_CS && offset > cpu.segReg[sreg].limit){
		printf("offset: %x\n", offset);
		printf("sreg: %d %x\n", sreg, cpu.segReg[sreg].limit);
		printf("Warning! offset > limit\n");
	}
	*/

	uint32_t laddr = cpu.segReg[sreg].base + offset;
	return laddr;
}

// load the invisible part of a segment register
void load_sreg(uint8_t sreg) {
	/* TODO: load the invisibile part of the segment register 'sreg' by reading the GDT.
	 * The visible part of 'sreg' should be assigned by mov or ljmp already.
	 */

	assert(cpu.segReg[sreg].ti == 0);

	if(cpu.segReg[sreg].ti == 0){
		uint32_t segDesc_addr = (cpu.segReg[sreg].index * 8) + cpu.gdtr.base;
		SegDesc sdt;
		sdt.val[0] = vaddr_read(segDesc_addr, SREG_CS, 4);
		sdt.val[1] = vaddr_read(segDesc_addr + 4, SREG_CS, 4);
		
		assert(sdt.present == 1);
		assert(sdt.granularity == 1);

		cpu.segReg[sreg].base = (sdt.base_15_0) | (sdt.base_23_16 << 16) | (sdt.base_31_24 << 24);
		cpu.segReg[sreg].limit = (sdt.limit_15_0) | (sdt.limit_19_16 << 16); 
		cpu.segReg[sreg].type = sdt.type;
		cpu.segReg[sreg].privilege_level = sdt.privilege_level;
		cpu.segReg[sreg].soft_use = sdt.soft_use;
	}
	
}

