#include "cpu/instr.h"

instr_func opcode_entry[256] = {
/* 0x00 - 0x03*/	add_r2rm_b, add_r2rm_v, add_rm2r_b, add_rm2r_v,
/* 0x04 - 0x07*/	add_i2a_b, add_i2a_v, inv, inv,
/* 0x08 - 0x0b*/	or_r2rm_b, or_r2rm_v, or_rm2r_b, or_rm2r_v,
/* 0x0c - 0x0f*/	or_i2a_b, or_i2a_v, inv, opcode_2_byte,
/* 0x10 - 0x13*/	adc_r2rm_b, adc_r2rm_v, adc_rm2r_b, adc_rm2r_v,
/* 0x14 - 0x17*/	adc_i2a_b, adc_i2a_v, inv, inv,
/* 0x18 - 0x1b*/	sbb_r2rm_b, sbb_r2rm_v, sbb_rm2r_b, sbb_rm2r_v,
/* 0x1c - 0x1f*/	sbb_i2a_b, sbb_i2a_v, inv, inv,
/* 0x20 - 0x23*/	and_r2rm_b, and_r2rm_v, and_rm2r_b, and_rm2r_v,
/* 0x24 - 0x27*/	and_i2a_b, and_i2a_v, inv, inv,
/* 0x28 - 0x2b*/	sub_r2rm_b, sub_r2rm_v, sub_rm2r_b, sub_rm2r_v,
/* 0x2c - 0x2f*/	sub_i2a_b, sub_i2a_v, inv, inv,
/* 0x30 - 0x33*/	xor_r2rm_b, xor_r2rm_v, xor_rm2r_b, xor_rm2r_v,
/* 0x34 - 0x37*/	xor_i2a_b, xor_i2a_v, inv, inv,
/* 0x38 - 0x3b*/	cmp_r2rm_b, cmp_r2rm_v, cmp_rm2r_b, cmp_rm2r_v,
/* 0x3c - 0x3f*/	cmp_i2a_b, cmp_i2a_v, inv, inv,
/* 0x40 - 0x43*/	inc_r_v, inc_r_v, inc_r_v, inc_r_v,
/* 0x44 - 0x47*/	inc_r_v, inc_r_v, inc_r_v, inc_r_v,
/* 0x48 - 0x4b*/	dec_r_v, dec_r_v, dec_r_v, dec_r_v,
/* 0x4c - 0x4f*/	dec_r_v, dec_r_v, dec_r_v, dec_r_v,
/* 0x50 - 0x53*/	push_r_v, push_r_v, push_r_v, push_r_v,
/* 0x54 - 0x57*/	push_r_v, push_r_v, push_r_v, push_r_v,
/* 0x58 - 0x5b*/	pop_r_v, pop_r_v, pop_r_v, pop_r_v,
/* 0x5c - 0x5f*/	pop_r_v, pop_r_v, pop_r_v, pop_r_v,
/* 0x60 - 0x63*/	pusha, popa, inv, inv,
/* 0x64 - 0x67*/	inv, inv, data_size_16, inv,
/* 0x68 - 0x6b*/	push_i_v, imul_irm2r_v, push_i_b, imul_i8rm2r_v,
/* 0x6c - 0x6f*/	inv, inv, inv, inv,
/* 0x70 - 0x73*/	jo_short_, jno_short_, jb_short_, jae_short_,
/* 0x74 - 0x77*/	je_short_, jne_short_, jna_short_, ja_short_,
/* 0x78 - 0x7b*/	js_short_, jns_short_, jp_short_, jnp_short_,
/* 0x7c - 0x7f*/	jl_short_, jge_short_, jle_short_, jg_short_,
/* 0x80 - 0x83*/	group_1_b, group_1_v, nemu_trap, group_1_bv,
/* 0x84 - 0x87*/	__ref_test_r2rm_b, __ref_test_r2rm_v, inv, inv,
/* 0x88 - 0x8b*/	__ref_mov_r2rm_b, __ref_mov_r2rm_v, __ref_mov_rm2r_b, __ref_mov_rm2r_v,
/* 0x8c - 0x8f*/	inv, __ref_lea, __ref_mov_rm2s_w, inv,
/* 0x90 - 0x93*/	nop, inv, inv, inv,
/* 0x94 - 0x97*/	inv, inv, inv, inv,
/* 0x98 - 0x9b*/	__ref_cbw_a_v, __ref_cltd, inv, inv,
/* 0x9c - 0x9f*/	inv, inv, __ref_sahf, inv,
/* 0xa0 - 0xa3*/	__ref_mov_o2a_b, __ref_mov_o2a_v, __ref_mov_a2o_b, __ref_mov_a2o_v,
/* 0xa4 - 0xa7*/	__ref_movs_b, __ref_movs_v, __ref_cmps_b, __ref_cmps_v,
/* 0xa8 - 0xab*/	__ref_test_i2a_b, __ref_test_i2a_v, __ref_stos_b, __ref_stos_v,
/* 0xac - 0xaf*/	inv, inv, inv, inv,
/* 0xb0 - 0xb3*/	mov_i2r_b, mov_i2r_b, mov_i2r_b, mov_i2r_b,
/* 0xb4 - 0xb7*/	mov_i2r_b, mov_i2r_b, mov_i2r_b, mov_i2r_b,
/* 0xb8 - 0xbb*/	mov_i2r_v, mov_i2r_v, mov_i2r_v, mov_i2r_v,
/* 0xbc - 0xbf*/	mov_i2r_v, mov_i2r_v, mov_i2r_v, mov_i2r_v,
/* 0xc0 - 0xc3*/	group_2_b, group_2_v, ret_near_imm16, ret_near,
/* 0xc4 - 0xc7*/	inv, inv, __ref_mov_i2rm_b, __ref_mov_i2rm_v,
/* 0xc8 - 0xcb*/	inv, __ref_leave, inv, inv,
/* 0xcc - 0xcf*/	inv, __ref_int_, inv, __ref_iret,
/* 0xd0 - 0xd3*/	group_2_1b, group_2_1v, group_2_cb, group_2_cv,
/* 0xd4 - 0xd7*/	inv, inv, inv, inv,
/* 0xd8 - 0xdb*/	group_x87_d8, group_x87_d9, group_x87_da, group_x87_db,
/* 0xdc - 0xdf*/	group_x87_dc, group_x87_dd, group_x87_de, group_x87_df,
/* 0xe0 - 0xe3*/	inv, inv, inv, __ref_jecxz_short_,
/* 0xe4 - 0xe7*/	in_i2r_b, in_i2r_v, out_r2i_b, out_r2i_v,
/* 0xe8 - 0xeb*/	__ref_call_near, __ref_jmp_near, __ref_jmp_far_imm, __ref_jmp_short,
/* 0xec - 0xef*/	__ref_in_b, __ref_in_v, __ref_out_b, __ref_out_v,
/* 0xf0 - 0xf3*/	inv, break_point, inv, __ref_rep_repe,
/* 0xf4 - 0xf7*/	__ref_hlt, inv, group_3_b, group_3_v,
/* 0xf8 - 0xfb*/	__ref_clc, inv, __ref_cli, __ref_sti,
/* 0xfc - 0xff*/	__ref_cld, inv, inv, group_5_indirect,
};

/* 0x80 */
instr_func group_1_b_entry[8] =
{add_i2rm_b, or_i2rm_b, adc_i2rm_b, sbb_i2rm_b, and_i2rm_b, sub_i2rm_b, xor_i2rm_b, cmp_i2rm_b};

/* 0x81 */
instr_func group_1_v_entry[8] =
{add_i2rm_v, or_i2rm_v, adc_i2rm_v, sbb_i2rm_v, and_i2rm_v, sub_i2rm_v, xor_i2rm_v, cmp_i2rm_v};

/* 0x83 */
instr_func group_1_bv_entry[8] =
{add_i2rm_bv, or_i2rm_bv, adc_i2rm_bv, sbb_i2rm_bv, and_i2rm_bv, sub_i2rm_bv, xor_i2rm_bv, cmp_i2rm_bv};

/* 0xc0 */
instr_func group_2_b_entry[8] =
{inv, inv, inv, inv, shl_i2rm_b, shr_i2rm_b, inv, sar_i2rm_b};

/* 0xc1 */
instr_func group_2_v_entry[8] =
{inv, inv, inv, inv, shl_i2rm_bv, shr_i2rm_bv, inv, sar_i2rm_bv};

/* 0xd0 */
instr_func group_2_1b_entry[8] =
{inv, inv, inv, inv, shl_rm_b, shr_rm_b, inv, sar_rm_b};

/* 0xd1 */
instr_func group_2_1v_entry[8] =
{inv, inv, inv, inv, shl_rm_v, shr_rm_v, inv, sar_rm_v};

/* 0xd2 */
instr_func group_2_cb_entry[8] =
{inv, inv, inv, inv, shl_c2rm_b, inv, shr_c2rm_b, sar_c2rm_b};

/* 0xd3 */
instr_func group_2_cv_entry[8] =
{inv, inv, inv, inv, shl_c2rm_bv, inv, shr_c2rm_bv, sar_c2rm_bv};

/* 0xf6 */
instr_func group_3_b_entry[8] =
{test_i2rm_b, inv, not_rm_b, neg_rm_b, mul_rm2a_b, imul_rm2a_b, __ref_div_rm2a_b, idiv_rm2a_b};

/* 0xf7 */
instr_func group_3_v_entry[8] =
{test_i2rm_v, inv, not_rm_v, neg_rm_v, mul_rm2a_v, imul_rm2a_v, __ref_div_rm2a_v, idiv_rm2a_v};

/* 0xff */
instr_func group_5_indirect_entry[8] =
{__ref_inc_rm_v, __ref_dec_rm_v, __ref_call_near_indirect, inv, __ref_jmp_near_indirect, inv, __ref_push_rm_v, inv};

instr_func group_7_entry[8] = 
{inv, inv, lgdt, lidt, inv, inv, inv, inv};

/* 0xd8 */
instr_func group_x87_d8_entry[8] =
{x87_fadds, x87_fmuls, inv, inv, x87_fsubs, inv, x87_fdivs, inv};

/* 0xd9 */
instr_func group_x87_d9_entry[8] =
{x87_flds, x87_fxch, x87_fsts, x87_fstps, inv, x87_fldx, inv, x87_fnstcw};

/* 0xda */
instr_func group_x87_da_entry[8] =
{inv, inv, inv, inv, inv, x87_fucompp, inv, inv};

/* 0xdb */
instr_func group_x87_db_entry[8] =
{x87_fildl, inv, inv, x87_fistpl, inv, x87_fucomi, inv, inv};

/* 0xdc */
instr_func group_x87_dc_entry[8] =
{x87_faddlx, x87_fmullx, inv, inv, inv, inv, inv, inv};

/* 0xdd */
instr_func group_x87_dd_entry[8] =
{x87_fldl, inv, x87_fstx, x87_fstpx, x87_fucom, x87_fucomp, inv, inv};

/* 0xde */
instr_func group_x87_de_entry[8] =
{x87_faddx, inv, inv, inv, inv, inv, inv, inv};

/* 0xdf */
instr_func group_x87_df_entry[8] =
{inv, inv, inv, inv, x87_fnstsw, x87_fucomip, inv, inv};

instr_func opcode_2_byte_entry[256] = {
/* 0x00 - 0x03*/        inv, group_7, inv, inv,
/* 0x04 - 0x07*/        inv, inv, inv, inv,
/* 0x08 - 0x0b*/        inv, inv, inv, inv,
/* 0x0c - 0x0f*/        inv, inv, inv, inv,
/* 0x10 - 0x13*/        inv, inv, inv, inv,
/* 0x14 - 0x17*/        inv, inv, inv, inv,
/* 0x18 - 0x1b*/        inv, inv, inv, inv,
/* 0x1c - 0x1f*/        inv, inv, inv, inv,
/* 0x20 - 0x23*/        mov_c2r_l, inv, mov_r2c_l, inv,
/* 0x24 - 0x27*/        inv, inv, inv, inv,
/* 0x28 - 0x2b*/        inv, inv, inv, inv,
/* 0x2c - 0x2f*/        inv, inv, inv, inv,
/* 0x30 - 0x33*/        inv, inv, inv, inv,
/* 0x34 - 0x37*/        inv, inv, inv, inv,
/* 0x38 - 0x3b*/        inv, inv, inv, inv,
/* 0x3c - 0x3f*/        inv, inv, inv, inv,
/* 0x40 - 0x43*/        inv, inv, inv, inv,
/* 0x44 - 0x47*/        cmove_rm2r_v, cmovne_rm2r_v, inv, cmova_rm2r_v,
/* 0x48 - 0x4b*/        cmovs_rm2r_v, cmovns_rm2r_v, inv, inv,
/* 0x4c - 0x4f*/        inv, cmovge_rm2r_v, inv, inv,
/* 0x50 - 0x53*/        inv, inv, inv, inv,
/* 0x54 - 0x57*/        inv, inv, inv, inv,
/* 0x58 - 0x5b*/        inv, inv, inv, inv,
/* 0x5c - 0x5f*/        inv, inv, inv, inv,
/* 0x60 - 0x63*/        inv, inv, inv, inv,
/* 0x64 - 0x67*/        inv, inv, inv, inv,
/* 0x68 - 0x6b*/        inv, inv, inv, inv,
/* 0x6c - 0x6f*/        inv, inv, inv, inv,
/* 0x70 - 0x73*/        inv, inv, inv, inv,
/* 0x74 - 0x77*/        inv, inv, inv, inv,
/* 0x78 - 0x7b*/        inv, inv, inv, inv,
/* 0x7c - 0x7f*/        inv, inv, inv, inv,
/* 0x80 - 0x83*/        __ref_jo_near, __ref_jno_near, __ref_jb_near, __ref_jae_near,
/* 0x84 - 0x87*/        __ref_je_near, __ref_jne_near, __ref_jna_near, __ref_ja_near,
/* 0x88 - 0x8b*/        __ref_js_near, __ref_jns_near, __ref_jp_near, __ref_jnp_near,
/* 0x8c - 0x8f*/        __ref_jl_near, __ref_jge_near, __ref_jle_near, __ref_jg_near,
/* 0x90 - 0x93*/        seto_b, setno_b, setc_b, setae_b,
/* 0x94 - 0x97*/        sete_b, setne_b, setbe_b, seta_b,
/* 0x98 - 0x9b*/        sets_b, setns_b, setp_b, setnp_b,
/* 0x9c - 0x9f*/        setl_b, setge_b, setle_b, setg_b,
/* 0xa0 - 0xa3*/        inv, inv, inv, bt_r2rm_v,
/* 0xa4 - 0xa7*/        inv, inv, inv, inv,
/* 0xa8 - 0xab*/        inv, inv, inv, inv,
/* 0xac - 0xaf*/        inv, inv, inv, imul_rm2r_v,
/* 0xb0 - 0xb3*/        inv, inv, inv, inv,
/* 0xb4 - 0xb7*/        inv, inv, mov_zrm82r_v, mov_zrm162r_l,
/* 0xb8 - 0xbb*/        inv, inv, inv, inv,
/* 0xbc - 0xbf*/        inv, inv, mov_srm82r_v, mov_srm162r_l,
/* 0xc0 - 0xc3*/        inv, inv, inv, inv,
/* 0xc4 - 0xc7*/        inv, inv, inv, inv,
/* 0xc8 - 0xcb*/        inv, inv, inv, inv,
/* 0xcc - 0xcf*/        inv, inv, inv, inv,
/* 0xd0 - 0xd3*/        inv, inv, inv, inv,
/* 0xd4 - 0xd7*/        inv, inv, inv, inv,
/* 0xd8 - 0xdb*/        inv, inv, inv, inv,
/* 0xdc - 0xdf*/        inv, inv, inv, inv,
/* 0xe0 - 0xe3*/        inv, inv, inv, inv,
/* 0xe4 - 0xe7*/        inv, inv, inv, inv,
/* 0xe8 - 0xeb*/        inv, inv, inv, inv,
/* 0xec - 0xef*/        inv, inv, inv, inv,
/* 0xf0 - 0xf3*/        inv, inv, inv, inv,
/* 0xf4 - 0xf7*/        inv, inv, inv, inv,
/* 0xf8 - 0xfb*/        inv, inv, inv, inv,
/* 0xfc - 0xff*/        inv, inv, inv, inv,
};


