/*
 * Copyright (C) 2014  Bin Meng <bmeng.cn@gmail.com>
 *
 * This file is adapted from coreboot src/arch/x86/boot/pirq_routing.c
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#include <common.h>
#include <asm/pirq_routing.h>

static int verify_pirq_routing_table(struct irq_routing_table *rt)
{
	uint8_t *addr = (uint8_t *)rt;
	uint8_t sum=0;
	int i;

	if (rt->signature != PIRQ_SIGNATURE || rt->version != PIRQ_VERSION ||
	    rt->size < 32 || rt->size % 16 ) {
		printf("Interrupt Routing Table not valid.\n");
		return -1;
	}

	printf("Checking Interrupt Routing Table consistency...\n");

	if (sizeof(struct irq_routing_table) != rt->size) {
		printf("Inconsistent Interrupt Routing Table size (0x%x/0x%x).\n",
			       (unsigned int) sizeof(struct irq_routing_table),
			       rt->size
			);
		rt->size=sizeof(struct irq_routing_table);
	}

	for (i = 0; i < rt->size; i++)
		sum += addr[i];

	sum = rt->checksum - sum;

	if (sum != rt->checksum) {
		printf("Interrupt Routing Table checksum is: 0x%02x but should be: 0x%02x.\n",
			       rt->checksum, sum);
		rt->checksum = sum;
	}

	printf("done.\n");
	return 0;
}

unsigned long write_pirq_routing_table(unsigned long addr, struct irq_routing_table *routing_table)
{
	/* Do a sanity test for the table and update the checksum automatically */
	verify_pirq_routing_table(routing_table);

	/* Align the table to be 16 byte aligned. */
	addr = ALIGN(addr, 16);

	/* This table must be between 0xf0000 & 0x100000 */
	printf("Copying Interrupt Routing Table to 0x%08lx... ", addr);
	memcpy((void *)addr, routing_table, routing_table->size);
	printf("done.\n");

	return addr + routing_table->size;
}
