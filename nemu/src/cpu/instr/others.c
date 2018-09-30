#include "cpu/instr.h"

make_instr_func(sub_i2rm_bv){
	OPERAND rm;
	rm.data_size = data_size;

	int len = 1;
	len += moderm_rm(eip + 1,&rm);

	return len + 1;
}
