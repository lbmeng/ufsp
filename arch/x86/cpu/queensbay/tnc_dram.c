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
#include <asm/e820.h>

#define CROWNBAY_DRAM_SIZE	(1024 * 1024 * 1024)

DECLARE_GLOBAL_DATA_PTR;

int dram_init_f(void)
{
	gd->ram_size = CROWNBAY_DRAM_SIZE;

	return 0;
}

void dram_init_banksize(void)
{
	if (CONFIG_NR_DRAM_BANKS) {
		gd->bd->bi_dram[0].start = 0;
		gd->bd->bi_dram[0].size = CROWNBAY_DRAM_SIZE;
	}
	return;
}

ulong board_get_usable_ram_top(ulong total_size)
{
	return GetUsableLowMemTop(gd->arch.hob_list);
}

unsigned install_e820_map(unsigned max_entries, struct e820entry *entries)
{
	unsigned num_entries = 4;

	entries[0].addr = 0;
	entries[0].size = 0xa0000;
	entries[0].type = E820_RAM;

	entries[1].addr = 0xa0000;
	entries[1].size = 0x60000;
	entries[1].type = E820_RESERVED;

	entries[2].addr = 0x100000;
	entries[2].size = 0x3bf00000;
	entries[2].type = E820_RAM;

	entries[3].addr = 0x3c000000;
	entries[3].size = 0x4000000;
	entries[3].type = E820_RESERVED;

	return num_entries;
}
