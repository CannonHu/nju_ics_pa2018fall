#include "cpu/cpu.h"
#include "memory/memory.h"

// return the linear address from the virtual address and segment selector
uint32_t segment_translate(uint32_t offset, uint8_t sreg) {
	/* TODO: perform segment translation from virtual address to linear address
	 * by reading the invisible part of the segment register 'sreg'
	 */
	return 0;
}

// load the invisible part of a segment register
void load_sreg(uint8_t sreg) {
	/* TODO: load the invisibile part of the segment register 'sreg' by reading the GDT.
	 * The visible part of 'sreg' should be assigned by mov or ljmp already.
	 */
	uint32_t segDesc_addr = (cpu.segReg[sreg].index * 64) + cpu.gdtr.base;
	SegDesc* sdt = segDesc_addr;
	cpu.segReg[sreg].base = (sdt->base_15_0) | (sdt->base_23_16 << 16) | (sdt->base_31_24 << 24);
	cpu.segReg[sreg].limit = (sdt->limit_15_0) | (sdt->limit_19_16 << 16); 
	0cpu.segReg[sreg].type = sdt->type;
	cpu.segReg[sreg].privilege_level = sdt->privilege_level;
	cpu.segReg[sreg].soft_use = sdt->soft_use;

}

