/*
 * (C) Copyright 2014
 * Bin Meng, bmeng.cn@gmail.com.
 *
 * (C) Copyright 2008
 * Graeme Russ, graeme.russ@gmail.com.
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

/*
 * 16-bit initialization code
 *
 * Nothing needs to be done here so just jump back
 */

.section .start16, "ax"
.code16
.globl board_init16
board_init16:
	jmp	board_init16_ret
