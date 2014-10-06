/*
 * (C) Copyright 2014
 * Bin Meng, bmeng.cn@gmail.com.
 *
 * (C) Copyright 2008,2009
 * Graeme Russ, <graeme.russ@gmail.com>
 *
 * (C) Copyright 2002
 * Daniel Engström, Omicron Ceti AB, <daniel@omicron.se>
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#include <common.h>
#include <asm/fsp/fsp_support.h>

#define CROWNBAY_DRAM_SIZE	(1024 * 1024 * 1024)

DECLARE_GLOBAL_DATA_PTR;

int dram_init_f(void)
{
	gd->ram_size = CROWNBAY_DRAM_SIZE;

	return 0;
}

int dram_init_banksize(void)
{
	if (CONFIG_NR_DRAM_BANKS) {
		gd->bd->bi_dram[0].start = 0;
		gd->bd->bi_dram[0].size = CROWNBAY_DRAM_SIZE;
	}
	return 0;
}

ulong board_get_usable_ram_top(ulong total_size)
{
	return GetUsableLowMemTop(gd->arch.hob_list);
}
