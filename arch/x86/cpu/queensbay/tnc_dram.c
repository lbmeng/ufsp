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

DECLARE_GLOBAL_DATA_PTR;

int dram_init_f(void)
{
	phys_size_t ram_size = 0;
	EFI_PEI_HOB_POINTERS hob;

	hob.Raw = gd->arch.hob_list;
	while (!END_OF_HOB_LIST(hob)) {
		if (hob.Header->HobType == EFI_HOB_TYPE_RESOURCE_DESCRIPTOR) {
			if (hob.ResourceDescriptor->ResourceType == EFI_RESOURCE_SYSTEM_MEMORY ||
			    hob.ResourceDescriptor->ResourceType == EFI_RESOURCE_MEMORY_RESERVED) {
				ram_size += hob.ResourceDescriptor->ResourceLength;
			}
		}
		hob.Raw = GET_NEXT_HOB(hob);
	}

	gd->ram_size = ram_size;

	return 0;
}

void dram_init_banksize(void)
{
	gd->bd->bi_dram[0].start = 0;
	gd->bd->bi_dram[0].size = gd->ram_size;
}

ulong board_get_usable_ram_top(ulong total_size)
{
	return GetUsableLowMemTop(gd->arch.hob_list);
}

unsigned install_e820_map(unsigned max_entries, struct e820entry *entries)
{
	unsigned num_entries = 0;

	EFI_PEI_HOB_POINTERS hob;

	hob.Raw = gd->arch.hob_list;

	while (!END_OF_HOB_LIST(hob)) {
		if (hob.Header->HobType == EFI_HOB_TYPE_RESOURCE_DESCRIPTOR) {
			if (hob.ResourceDescriptor->ResourceType == EFI_RESOURCE_SYSTEM_MEMORY) {
				entries[num_entries].addr = hob.ResourceDescriptor->PhysicalStart;
				entries[num_entries].size = hob.ResourceDescriptor->ResourceLength;
				entries[num_entries].type = E820_RAM;
			} else if (hob.ResourceDescriptor->ResourceType == EFI_RESOURCE_MEMORY_RESERVED) {
				entries[num_entries].addr = hob.ResourceDescriptor->PhysicalStart;
				entries[num_entries].size = hob.ResourceDescriptor->ResourceLength;
				entries[num_entries].type = E820_RESERVED;
			}

		}
		hob.Raw = GET_NEXT_HOB(hob);
		num_entries++;
	}

	return num_entries;
}
