/*
 * (C) Copyright 2014
 * Bin Meng, bmeng.cn@gmail.com.
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#include <common.h>
#include <command.h>
#include <linux/compiler.h>
#include <asm/cpu.h>

static void display_cpuid(void)
{
	return;
}

int do_cpuid(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
	if (cpu_have_cpuid()) {
		printf("This CPU has CPUID support\n");
		display_cpuid();
	}
	else {
		printf("This CPU does not have CPUID support\n");
	}

	return 0;
}

/* -------------------------------------------------------------------- */

U_BOOT_CMD(
	cpuid,	1,	1,	do_cpuid,
	"print CPUID information",
	""
);
