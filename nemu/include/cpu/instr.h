#ifndef __INSTRUCTIONS_H__
#define __INSTRUCTIONS_H__

#include "nemu.h"
#include "cpu/cpu.h"
#include "cpu/fpu.h"
#include "cpu/modrm.h"
#include "cpu/operand.h"
#include "cpu/instr_helper.h"
#include "memory/memory.h"
#include <cpu-ref/instr_ref.h>

#include <stdio.h>
 
extern uint8_t data_size;

#include "cpu/instr/mov.h"
#include "cpu/instr/jmp.h"
#include "cpu/instr/shift.h"
#include "cpu/instr/flags.h"
#include "cpu/instr/group.h"
#include "cpu/instr/special.h"
#include "cpu/instr/x87.h"

/* TODO: add more instructions here */
make_instr_func(sub_i2rm_bv){
	OPERAND rm, imm;
	rm.data_size = data_size;
	imm.data_size = data_size;
	int len = 1;
	len += modrm_rm(eip + 1,&rm);
	
	imm.type = OPR_IMM;
	imm.addr = eip + len;
	operand_read(&imm);
	rm.val = imm.val;
	operand_write(&rm);
	return len;
}
#endif
