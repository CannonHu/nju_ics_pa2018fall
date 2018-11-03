#include "nemu.h"
#include "memory/memory.h"
#include "memory/cache.h"

#include <stdio.h>
#include <stdlib.h>


void init_cache(){
	for(int i = 0; i < SLOT_NUM; i++){
		for(int j = 0; j < LINE_IN_SLOT; j++){
			cache[i][j].valid = 0;
			cache[i][j].sign = 0;
			memset(cache[i][j].data_cell, 0, line_data_size);
		}
	}
}


uint8_t get_slot(paddr_t paddr){
	uint8_t id = (paddr >> 6) & 0x7f;
	return id;
}

uint32_t get_line_sign(paddr_t paddr){
	uint32_t sign = (paddr >> 13) & 0x7ffff;
	return sign;
}

void memtocache(paddr_t paddr, uint8_t slot_id, uint8_t line_id){
	memcpy(cache[slot_id][line_id].data_cell, hw_mem + (paddr & 0xffffffc0), line_data_size);
	cache[slot_id][line_id].valid = 1;	
}

uint32_t cache_read_line(paddr_t paddr, uint8_t slot_id, uint32_t line_sign, size_t len){
	uint32_t cell_num = paddr & 0x3f;
	uint32_t ret = 0;

	for(int j = 0; j < LINE_IN_SLOT; j++){
		if(cache[slot_id][j].sign == line_sign){
			if(!cache[slot_id][j].valid){
				memtocache(paddr, slot_id, j);
			}
			memcpy(&ret, cache[slot_id][j].data_cell + cell_num, len);
			return ret;
		}
	}
	for(int j = 0; j < LINE_IN_SLOT; j++){
		if(!cache[slot_id][j].valid){
			memtocache(paddr, slot_id, j);
			cache[slot_id][j].sign = line_sign;				
		}
		memcpy(&ret, cache[slot_id][j].data_cell + cell_num, len);
		return ret;
	}	
	uint8_t line_id = rand() % 8;
	memtocache(paddr, slot_id, line_id);
	cache[slot_id][line_id].sign = line_sign;
	memcpy(&ret, cache[slot_id][line_id].data_cell + cell_num, len);
	return ret;
}

uint32_t cache_read(paddr_t paddr, size_t len){
	printf("paddr read: %x\n", paddr);
	uint32_t ret = 0;
	paddr_t addrn = paddr;
	uint32_t slot_id = get_slot(addrn);
	uint32_t line_sign = get_line_sign(addrn);
	uint32_t cell_num = paddr & 0x3f;
	if(cell_num + len < line_data_size){
		return   cache_read_line(addrn, slot_id, line_sign, len);		
	}
	else{
		int firstlen = line_data_size - cell_num;
		int seclen = len - firstlen;
		ret = cache_read_line(addrn, slot_id, line_sign, firstlen);
		addrn = (paddr & 0xffffffc0) + 0x40;
		slot_id ++;
		ret += cache_read_line(addrn, slot_id, line_sign, seclen) << (firstlen * 8);
		return ret;

	}	

}


void cache_write_line(paddr_t paddr, uint8_t slot_id, uint32_t line_sign, uint32_t data, size_t len){
	uint32_t cell_num = paddr & 0x3f;
	for(int j = 0; j < LINE_IN_SLOT; j++){
		if(cache[slot_id][j].sign == line_sign){
			memcpy(cache[slot_id][j].data_cell + cell_num, &data, len);
			return;
		}
	}

}


void cache_write(paddr_t paddr, size_t len, uint32_t data){
	printf("paddr write: %x\n",paddr);
	paddr_t addrn = paddr;
	uint32_t slot_id = get_slot(addrn);
	uint32_t line_sign = get_line_sign(addrn);
	uint32_t cell_num = paddr & 0x3f;
	if(cell_num + len < line_data_size){
		cache_write_line(addrn, slot_id, line_sign, data, len);
	}
	else{
		int firstlen = line_data_size - cell_num;
		int seclen = len - firstlen;
		cache_write_line(addrn, slot_id, line_sign, data, firstlen);
		addrn = (paddr & 0xffffffc0) + 0x40;
		slot_id ++;
		cache_write_line(addrn, slot_id, line_sign, data >> (firstlen * 8), seclen);
	}
	memcpy(hw_mem + paddr, &data, len);
}
