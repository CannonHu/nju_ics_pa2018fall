#include "nemu.h"


typedef struct{
	bool valid;
	uint32_t sign;
	uint8_t data_cell[64];
}CacheLine;

void init_cache();

uint32_t cache_read(paddr_t paddr, size_t len, CacheLine* cache);

void cache_write(paddr_t paddr, size_t len, uint32_t data, CacheLine* cache);
