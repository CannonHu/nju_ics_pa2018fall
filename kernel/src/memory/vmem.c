#include "common.h"
#include "memory.h"
#include <string.h>

#define VMEM_ADDR 0xa0000
#define SCR_SIZE (320 * 200)
#define NR_PT ((SCR_SIZE + PT_SIZE - 1) / PT_SIZE)  // number of page tables to cover the vmem

PTE mptable[NR_PT] align_to_page;
PDE* get_updir();

void create_video_mapping() {
	
	/* TODO: create an identical mapping from virtual memory area
	 * [0xa0000, 0xa0000 + SCR_SIZE) to physical memeory area
	 * [0xa0000, 0xa0000 + SCR_SIZE) for user program. You may define
	 * some page tables to create this mapping.
	 */
	
	PDE *pdir = get_updir();
	Log("pdir: %x\n", (uint32_t)pdir);
	PTE *ptable = (PTE*)va_to_pa(mptable);
	uint32_t pdir_idx, ptable_idx, pframe_idx;
	pframe_idx = 0xa0000;
	ptable_idx = 0xa0;

	for(pdir_idx = 0; pdir_idx < SCR_SIZE/PT_SIZE; pdir_idx ++){
		BREAK_POINT
		pdir[pdir_idx].val = make_pde(ptable);
		Log("pdir-val: %x", pdir[pdir_idx].val);
		for(; ptable_idx < NR_PTE; ptable_idx++){
			if(pframe_idx >= 0xafa00){
				break;
			}
			ptable->val = make_pte(pframe_idx << 12);
			pframe_idx ++;
			ptable ++;
		}
	}
	

	//panic("please implement me");
}

void video_mapping_write_test() {
	int i;
	uint32_t *buf = (void *)VMEM_ADDR;
	for(i = 0; i < SCR_SIZE / 4; i ++) {
		buf[i] = i;
	}
}

void video_mapping_read_test() {
	int i;
	uint32_t *buf = (void *)VMEM_ADDR;
	for(i = 0; i < SCR_SIZE / 4; i ++) {
		if(buf[i] != i){
			Log("wrong i: %d", i);
		}
		assert(buf[i] == i);
	}
}

void video_mapping_clear() {
	memset((void *)VMEM_ADDR, 0, SCR_SIZE);
}

