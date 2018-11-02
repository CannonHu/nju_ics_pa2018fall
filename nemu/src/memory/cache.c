#include "nemu.h"
#include "memory/memory.h"

#include <stdio.h>

CacheLine cache[128][8];

init_cache(){
	for(int i = 0; i < 128; i++)
		for(int j = 0; j < 8 ;j++){
			cache[i][j].valid = 0;
			cache[i][j].sign = 0;
			memset(cache[i][j].data_cell, 0, 64);
		}
}
