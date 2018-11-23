#include "nemu.h"
#include "cpu/cpu.h"
#include "memory/memory.h"
#include "memory/cache.h"
#include "device/mm_io.h"
#include <memory.h>
#include <stdio.h>


uint8_t hw_mem[MEM_SIZE_B];

uint32_t hw_mem_read(paddr_t paddr, size_t len) {
	uint32_t ret = 0;
	memcpy(&ret, hw_mem + paddr, len);
	return ret;
}

void hw_mem_write(paddr_t paddr, size_t len, uint32_t data) {
	memcpy(hw_mem + paddr, &data, len);
}

uint32_t paddr_read(paddr_t paddr, size_t len) {
	uint32_t ret = 0;
#ifdef CACHE_ENABLED
	ret = cache_read(paddr, len, cache[0]);
#else
	ret = hw_mem_read(paddr, len);
	mem_count += 10;
#endif
	return ret;
}

void paddr_write(paddr_t paddr, size_t len, uint32_t data) {
#ifdef CACHE_ENABLED
	cache_write(paddr, len, data, cache[0]);
#else
	hw_mem_write(paddr, len, data);
	mem_count += 10;
#endif

}


uint32_t laddr_read(laddr_t laddr, size_t len) {
	assert(len == 1 || len == 2 || len == 4);
#ifdef IA32_PAGE
	laddr_t paddr = laddr;
	if(cpu.cr0.pg){
		size_t len1 = ((paddr + len) >> 12 << 12) - paddr;
		if(len1 < len){
			printf("special twin-page\n");
			uint32_t ret_val = 0;
			paddr = page_translate(paddr);
			ret_val = paddr_read(paddr, len1);
			paddr = laddr + len1;
			paddr = page_translate(paddr);
			ret_val |= paddr_read(paddr, len - len1) << (len1 * 8);
			return ret_val;
		}
		else{
			paddr = page_translate(paddr);
			return paddr_read(paddr, len);
		}
	}
	return paddr_read(laddr, len);

#else
	return paddr_read(laddr, len);
#endif
}

void laddr_write(laddr_t laddr, size_t len, uint32_t data) {
	assert(len == 1 || len == 2 || len == 4);
#ifdef IA32_PAGE
	laddr_t paddr = laddr;
	if(cpu.cr0.pg){
		size_t len1 = ((paddr + len) >> 12 << 12) - paddr;
		if(len1 < len){
			paddr = page_translate(paddr);
			paddr_write(paddr, len1, data);
			paddr = laddr + len1;
			paddr = page_translate(paddr);
			paddr_write(paddr, len - len1, data >> (len1 * 8));
		}
		else{
			paddr = page_translate(paddr);
			return paddr_write(paddr, len, data);
		}
	}
	paddr_write(paddr, len, data);

#else
	paddr_write(laddr, len, data);
#endif
}


uint32_t vaddr_read(vaddr_t vaddr, uint8_t sreg, size_t len) {
	assert(len == 1 || len == 2 || len == 4);
#ifndef IA32_SEG
	return laddr_read(vaddr, len);
#else
	uint32_t laddr = vaddr;
	if(cpu.cr0.pe){
		laddr = segment_translate(vaddr, sreg);
	}
	return laddr_read(laddr, len);
#endif
}

void vaddr_write(vaddr_t vaddr, uint8_t sreg, size_t len, uint32_t data) {
	assert(len == 1 || len == 2 || len == 4);
#ifndef IA32_SEG
	return laddr_write(vaddr, len, data);
#else
	uint32_t laddr = vaddr;
	if(cpu.cr0.pe){
		laddr = segment_translate(vaddr, sreg);
	}
	return laddr_write(laddr, len, data);
#endif
}

void init_mem() {
	// clear the memory on initiation
#ifdef CACHE_ENABLED
	init_cache();
#endif
	memset(hw_mem, 0, MEM_SIZE_B);

#ifdef TLB_ENABLED
	make_all_tlb();
	init_all_tlb();
#endif
}

uint32_t instr_fetch(vaddr_t vaddr, size_t len) {
	assert(len == 1 || len == 2 || len == 4);
	return vaddr_read(vaddr, SREG_CS, len);
}

uint8_t * get_mem_addr() {
	return hw_mem;
}
