#include "nemu.h"

typedef struct{
	bool valid;
	uint32_t sign;
	struct{
		uint8_t data_cell[64];
	}DataBlock

}CacheLine
