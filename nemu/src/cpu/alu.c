#include "cpu/cpu.h"

void set_CF_add(uint32_t result,uint32_t src,size_t data_size){
	result = sign_ext(result & (0xFFFFFFFF >> (32 - data_size)),data_size);
	src = sign_ext(src & (0xFFFFFFFF >> (32 - data_size)),data_size);
	cpu.eflags.CF = result<src;	
}

void set_CF_adc(uint32_t result,uint32_t src,uint32_t dest,size_t data_size){

	if((src == 0x00000000 || dest == 0x00000000) && cpu.eflags.CF == 0){
		cpu.eflags.CF = 0;
		return;
	}
	result = sign_ext(result & (0xFFFFFFFF >> (32 - data_size)),data_size);
	src = sign_ext(src & (0xFFFFFFFF >> (32 - data_size)),data_size);
	dest = sign_ext(dest & (0xFFFFFFFF >> (32 - data_size)),data_size);
	if(cpu.eflags.CF == 1)
		cpu.eflags.CF=result<=src;
	else
		cpu.eflags.CF=result<src;
}

void set_CF_sub(uint32_t result,uint32_t src,uint32_t dest,size_t data_size){
	result = sign_ext(result & (0xFFFFFFFF >> (32 - data_size)),data_size);
	src = sign_ext(src & (0xFFFFFFFF >> (32 - data_size)),data_size);
	dest = sign_ext(dest & (0xFFFFFFFF >> (32 - data_size)),data_size);
	cpu.eflags.CF=result>dest;

}

void set_CF_sbb(uint32_t result,uint32_t src,uint32_t dest,size_t data_size){

	result = sign_ext(result & (0xFFFFFFFF >> (32 - data_size)),data_size);
	src = sign_ext(src & (0xFFFFFFFF >> (32 - data_size)),data_size);
	dest = sign_ext(dest & (0xFFFFFFFF >> (32 - data_size)),data_size);
	if(cpu.eflags.CF == 1)
		cpu.eflags.CF=result>=dest;
	else
		cpu.eflags.CF=result>dest;
}


void set_ZF(uint32_t result,size_t data_size){
	result = result & (0xFFFFFFFF >> (32 - data_size));
	cpu.eflags.ZF=(result==0);
}

void set_SF(uint32_t result,size_t data_size){
	result = sign_ext(result & (0xFFFFFFFF >> (32-data_size)),data_size);
	cpu.eflags.SF=sign(result);
}


void set_PF(uint32_t result){
	int counts = 0;int tool = 0x00000001;
	for(int i = 0;i < 8;i++){
		if(tool & result){
			counts++;
		}
		tool = tool << 1;
	}
	cpu.eflags.PF = (counts + 1) % 2;
}

void set_OF_add(uint32_t result,uint32_t src,uint32_t dest,size_t data_size){
	switch(data_size){
		case 8:
			result = sign_ext(result & 0xFF,8);
			src = sign_ext(src & 0xFF,8);
			dest = sign_ext(dest & 0xFF,8);
			break;
		case 16:
			result = sign_ext(result & 0xFFFF,16);
			src = sign_ext(src & 0xFFFF,16);
			dest = sign_ext(dest & 0xFFFF,16);
			break;
		default:break;
	}
	if(sign(src) == sign(dest)){
		if(sign(src) != sign(result))
			cpu.eflags.OF = 1;
		else
			cpu.eflags.OF = 0;
	}
	else{
		cpu.eflags.OF = 0;
	}
}

void set_OF_sub(uint32_t result,uint32_t src,uint32_t dest,size_t data_size){
	switch(data_size){
		case 8:
			result = sign_ext(result & 0xFF,8);
			src = sign_ext(src & 0xFF,8);
			dest = sign_ext(dest & 0xFF,8);
			break;
		case 16:
			result = sign_ext(result & 0xFFFF,16);
			src = sign_ext(src & 0xFFFF,16);
			dest = sign_ext(dest & 0xFFFF,16);
			break;
		default:break;
	}
	if(sign(src) != sign(dest)){
		if(sign(dest) != sign(result))
			cpu.eflags.OF = 1;
		else
			cpu.eflags.OF = 0;
	}
	else{
		cpu.eflags.OF = 0;
	}
}

uint32_t alu_add(uint32_t src, uint32_t dest, size_t data_size) {
#ifdef NEMU_REF_ALU
	return __ref_alu_add(src, dest, data_size);
#else
	uint32_t res = 0;
	res = dest + src;
	
	set_CF_add(res,src,data_size);
	set_PF(res);

	set_ZF(res,data_size);
	set_SF(res,data_size);
	set_OF_add(res,src,dest,data_size);
	return res & (0xFFFFFFFF >> (32-data_size));
#endif
}

uint32_t alu_adc(uint32_t src, uint32_t dest, size_t data_size) {
#ifdef NEMU_REF_ALU
	return __ref_alu_adc(src, dest, data_size);
#else
	uint32_t res = 0;
	res = dest + src + cpu.eflags.CF;
	
	set_CF_adc(res,src,dest,data_size);
	set_PF(res);

	set_ZF(res,data_size);
	set_SF(res,data_size);
	set_OF_add(res,src,dest,data_size);
	return res & (0xFFFFFFFF >> (32-data_size));
#endif
}


uint32_t alu_sub(uint32_t src, uint32_t dest, size_t data_size) {
#ifdef NEMU_REF_ALU
	return __ref_alu_sub(src, dest, data_size);
#else
	uint32_t res = 0;
	uint32_t src_t = ~src + 1;
	res = dest + src_t;
	
	set_CF_sub(res,src,dest,data_size);
	set_PF(res);

	set_ZF(res,data_size);
	set_SF(res,data_size);
	set_OF_sub(res,src,dest,data_size);
	return res & (0xFFFFFFFF >> (32-data_size));
#endif
}

uint32_t alu_sbb(uint32_t src, uint32_t dest, size_t data_size) {
#ifdef NEMU_REF_ALU
	return __ref_alu_sbb(src, dest, data_size);
#else
	uint32_t res = 0;
	res = dest - src - cpu.eflags.CF;
	
	set_CF_sbb(res,src,dest,data_size);
	set_PF(res);

	set_ZF(res,data_size);
	set_SF(res,data_size);
	set_OF_sub(res,src,dest,data_size);
	return res & (0xFFFFFFFF >> (32-data_size));

#endif
}


uint64_t alu_mul(uint32_t src, uint32_t dest, size_t data_size) {
#ifdef NEMU_REF_ALU
	return __ref_alu_mul(src, dest, data_size);
#else
	uint64_t res = 0;
	printf("%d %x %x\n",data_size,dest,src);

	dest = dest & (0xFFFFFFFF >> (32-data_size));
	src = src & (0xFFFFFFFF >> (32-data_size));
	printf("%x %x\n",dest,src);
	res = dest * src;
	uint64_t t = res & (0xFFFFFFFFFFFFFFF >> (64 - data_size * 2));
	printf("%llx\n",t);
	return t;
#endif
}

int64_t alu_imul(int32_t src, int32_t dest, size_t data_size) {
#ifdef NEMU_REF_ALU
	return __ref_alu_imul(src, dest, data_size);
#else
	printf("\e[0;31mPlease implement me at alu.c\e[0m\n");
	assert(0);
	return 0;
#endif
}

uint32_t alu_div(uint64_t src, uint64_t dest, size_t data_size) {
#ifdef NEMU_REF_ALU
	return __ref_alu_div(src, dest, data_size);
#else
	printf("\e[0;31mPlease implement me at alu.c\e[0m\n");
	assert(0);
	return 0;
#endif
}

int32_t alu_idiv(int64_t src, int64_t dest, size_t data_size) {
#ifdef NEMU_REF_ALU
	return __ref_alu_idiv(src, dest, data_size);
#else
	printf("\e[0;31mPlease implement me at alu.c\e[0m\n");
	assert(0);
	return 0;
#endif
}

uint32_t alu_mod(uint64_t src, uint64_t dest) {
#ifdef NEMU_REF_ALU
	return __ref_alu_mod(src, dest);
#else
	printf("\e[0;31mPlease implement me at alu.c\e[0m\n");
	assert(0);
	return 0;
#endif
}

int32_t alu_imod(int64_t src, int64_t dest) {
#ifdef NEMU_REF_ALU
	return __ref_alu_imod(src, dest);
#else
	printf("\e[0;31mPlease implement me at alu.c\e[0m\n");
	assert(0);
	return 0;
#endif
}

uint32_t alu_and(uint32_t src, uint32_t dest, size_t data_size) {
#ifdef NEMU_REF_ALU
	return __ref_alu_and(src, dest, data_size);
#else
	uint32_t res = 0;
	res = dest & src;

	cpu.eflags.CF = 0;
	set_PF(res);

	set_ZF(res,data_size);
	set_SF(res,data_size);
	cpu.eflags.OF = 0;	
	return res & (0xFFFFFFFF >> (32-data_size));


#endif
}

uint32_t alu_xor(uint32_t src, uint32_t dest, size_t data_size) {
#ifdef NEMU_REF_ALU
	return __ref_alu_xor(src, dest, data_size);
#else
	uint32_t res = 0;
	res = dest ^ src;

	cpu.eflags.CF = 0;
	set_PF(res);

	set_ZF(res,data_size);
	set_SF(res,data_size);
	cpu.eflags.OF = 0;	
	return res & (0xFFFFFFFF >> (32-data_size));

#endif
}

uint32_t alu_or(uint32_t src, uint32_t dest, size_t data_size) {
#ifdef NEMU_REF_ALU
	return __ref_alu_or(src, dest, data_size);
#else
	uint32_t res = 0;
	res = dest | src;

	cpu.eflags.CF = 0;
	set_PF(res);

	set_ZF(res,data_size);
	set_SF(res,data_size);
	cpu.eflags.OF = 0;	
	return res & (0xFFFFFFFF >> (32-data_size));
#endif
}

uint32_t alu_shl(uint32_t src, uint32_t dest, size_t data_size) {
#ifdef NEMU_REF_ALU
	return __ref_alu_shl(src, dest, data_size);
#else
	uint32_t res = 0;
	dest = dest & (0xFFFFFFFF >> (32-data_size));
	res = dest << src;
	
	cpu.eflags.CF = (dest >> (data_size - src));	
	set_PF(res);

	set_ZF(res,data_size);
	set_SF(res,data_size);
	return res & (0xFFFFFFFF >> (32-data_size));

#endif
}

uint32_t alu_shr(uint32_t src, uint32_t dest, size_t data_size) {
#ifdef NEMU_REF_ALU
	return __ref_alu_shr(src, dest, data_size);
#else	
	uint32_t res = 0;
	dest = dest & (0xFFFFFFFF >> (32-data_size));
	res = dest >> src;
	
	cpu.eflags.CF = dest >> (src-1);
	set_PF(res);

	set_ZF(res,data_size);
	set_SF(res,data_size);
	return res & (0xFFFFFFFF >> (32-data_size));

#endif
}

uint32_t alu_sar(uint32_t src, uint32_t dest, size_t data_size) {
#ifdef NEMU_REF_ALU
	return __ref_alu_sar(src, dest, data_size);	
#else
	uint32_t res = 0;	
	dest = dest & (0xFFFFFFFF >> (32-data_size));
	res = (int)(dest << (32-data_size)) >> (int)src;
	res = res >> (32-data_size);
	cpu.eflags.CF = dest >> (src-1);
	set_PF(res);

	set_ZF(res,data_size);
	set_SF(res,data_size);
	return res & (0xFFFFFFFF >> (32-data_size));

#endif
}

uint32_t alu_sal(uint32_t src, uint32_t dest, size_t data_size) {
#ifdef NEMU_REF_ALU
	return __ref_alu_sal(src, dest, data_size);
#else
	uint32_t res = 0;
	dest = dest & (0xFFFFFFFF >> (32-data_size));
	res = dest << src;
	
	cpu.eflags.CF = (dest >> (data_size - src));	
	set_PF(res);

	set_ZF(res,data_size);
	set_SF(res,data_size);
	return res & (0xFFFFFFFF >> (32-data_size));
#endif
}
