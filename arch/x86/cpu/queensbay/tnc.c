/*
 * (C) Copyright 2014
 * Bin Meng, bmeng.cn@gmail.com.
 *
 * Copyright (c) 2011 The Chromium OS Authors.
 * (C) Copyright 2008
 * Graeme Russ, graeme.russ@gmail.com.
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#include <common.h>
#include <asm/io.h>

/*
 * Miscellaneous platform dependent initializations
 */
int cpu_init_f(void)
{
#ifdef CONFIG_SYS_X86_TSC_TIMER
	timer_set_base(rdtsc());
#endif

	return x86_cpu_init_f();
}

void reset_cpu(ulong addr)
{
	/* cold reset */
	outb(0x06, 0xcf9);
}
