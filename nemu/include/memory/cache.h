#include "nemu.h"

#define line_data_size = 64;

typedef struct{
	bool valid;
	uint32_t sign;
	uint8_t data_cell[line_data_size];
}CacheLine;

void init_cache();

uint32_t cache_read(paddr_t paddr, size_t len, CacheLine* cache);

void cache_write(paddr_t paddr, size_t len, uint32_t data, CacheLine* cache);
