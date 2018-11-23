#include "cpu/cpu.h"
#include "memory/memory.h"

// translate from linear address to physical address
paddr_t page_translate(laddr_t laddr) {
#ifndef TLB_ENABLED
	printf("used page! %x\n", laddr);
	uint32_t pde_addr = (cpu.cr3.pdbr << 12) + ((laddr >> 22) << 2);
	printf("pde_start: %x pde_addr: %x\n", cpu.cr3.pdbr, pde_addr);
	PDE cur_pde;
       	cur_pde.val = paddr_read(pde_addr, 4);
	//printf("cur_pde val %x\n",cur_pde.val);
	assert(cur_pde.present == 1);
	uint32_t pte_addr = (cur_pde.page_frame << 12) + ((laddr << 10 >> 22) << 2);
	PTE cur_pte;
        cur_pte.val = paddr_read(pte_addr, 4);
	assert(cur_pte.present == 1);
	return (cur_pte.page_frame << 12) + (laddr & 0xfff); 
#else	
	return tlb_read(laddr) | (laddr & PAGE_MASK);
#endif
}
