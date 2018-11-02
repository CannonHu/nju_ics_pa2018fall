#include "nemu.h"
#include "memory/memory.h"

#include <stdio.h>

#define LINE_IN_SLOT = 8
#define SLOT_NUM = 128

CacheLine cache[SLOT_NUM][LINE_IN_SLOT];

void init_cache(){
	for(int i = 0; i < 128; i++){
		for(int j = 0; j < 8 ;j++){
			cache[i][j].valid = 0;
			cache[i][j].sign = 0;
			memset(cache[i][j].data_cell, 0, 64);
		}
	}
}

uint8_t get_slot(paddr_t paddr){
	uint8_t id = (paddr >> 6) & 0x7f;
	return id;
}

uint32_t get_line_sign(paddr_t paddr){
	uint32_t sign = (paddr >> 13) & 0x7ffff;
}

uint32_t cache_read(paddr_t paddr, size_t len, CacheLine* cache){
	paddr_t addrn = paddr;
	uint8_t slot_id = 0;
	for(int i = 0; i < len; i++){
		
	}	
}
