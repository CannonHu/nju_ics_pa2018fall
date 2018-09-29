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
/*make_instr_func(add_r2rm_b){
	OPERAND r, rm;
	r.data_size = 8;
	rm.data_size = 8;
	int len = 1;
	len += modrm_r_rm(eip + 1, &r, &rm);
	operand_read(&r);
	rm.val = alu_add(r.val, rm.val, 8);
	operand_write(&rm);
	return len;
}*/
#endif
