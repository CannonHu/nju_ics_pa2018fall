#include "cpu/cpu.h"
#include "memory/memory.h"

// translate from linear address to physical address
paddr_t page_translate(laddr_t laddr) {
#ifndef TLB_ENABLED
	uint32_t pde_addr = (cpu.cr3.pdbr << 12) + (laddr >> 22);
	PDE cur_pde = paddr_read(pde_addr, 4);
	uint32_t pte_addr = (cur_pde << 12) + (laddr << 10 >> 22);
	PTE cur_pte = paddr_read(pte_addr, 4);
	return (cur_pte << 12) + (laddr & 0xfff); 
#else	
	return tlb_read(laddr) | (laddr & PAGE_MASK);;
#endif
}
