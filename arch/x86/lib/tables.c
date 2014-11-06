/*
 * Copyright (C) 2014  Bin Meng <bmeng.cn@gmail.com>
 *
 * This file is adapted from coreboot src/arch/x86/boot/tables.c
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#include <common.h>
#include <asm/tables.h>
#include <asm/pirq_routing.h>

extern struct irq_routing_table * pir_table;

void write_tables(void)
{
	unsigned long rom_table_end;

	rom_table_end = ROM_TABLE_LOW;

#ifdef CONFIG_GENERATE_PIRQ_TABLE
	/* This table must be between 0x0f0000 and 0x100000 */
	rom_table_end = write_pirq_routing_table(rom_table_end, pir_table);
	rom_table_end = ALIGN(rom_table_end, 1024);
#endif
}
