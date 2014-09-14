/*
 * (C) Copyright 2014
 * Bin Meng, bmeng.cn@gmail.com.
 *
 * Copyright (c) 2011 The Chromium OS Authors.
 * (C) Copyright 2010-2011
 * Graeme Russ, <graeme.russ@gmail.com>
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#include <config.h>

.globl car_init
car_init:

	lea	find_fsp_header_stack, %esp
	jmp	FindFspHeader

find_fsp_header_ret:

	/* EAX points to FSP_INFO_HEADER */
	mov	%eax, %ebp
	/* sanity test */
	cmp	$CONFIG_FSP_LOCATION, %eax
	jb	die

	/* calculate TempRamInitEntry address */
	mov	0x30(%ebp), %eax
	add	0x1c(%ebp), %eax

	/* call FSP TempRamInitEntry to setup temporary stack */
	lea	temp_ram_init_stack, %esp
	jmp	*%eax

temp_ram_init_ret:
	addl	$4, %esp
	cmp	$0, %eax
	jz	continue

die:
	hlt
	jmp	die
	hlt

continue:
	jmp	car_init_ret

	.align	4
find_fsp_header_stack:
	.long	find_fsp_header_ret

	.align	4
temp_ram_init_stack:
	.long	temp_ram_init_ret
	.long	temp_ram_init_params
temp_ram_init_params:
	.long	__ucode_start		/* microcode base */
	.long	__ucode_size		/* microcode size */
	.long	CONFIG_SYS_MONITOR_BASE	/* code region base */
	.long	CONFIG_SYS_MONITOR_LEN	/* code region size */
