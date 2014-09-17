/*
 * (C) Copyright 2014
 * Bin Meng, bmeng.cn@gmail.com.
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#include <common.h>
#include <command.h>
#include <linux/compiler.h>
#include <asm/fsp/fsp_support.h>

DECLARE_GLOBAL_DATA_PTR;

#define HOB_TYPE_MIN	0
#define HOB_TYPE_MAX	11

static char * hob_type[] = {
	"reserved",
	"Hand-off",
	"Memory Allocation",
	"Resource Descriptor",
	"GUID Extension",
	"Firmware Volumn",
	"CPU",
	"Memory Pool",
	"reserved",
	"Firmware Volumn 2",
	"Load PEIM Unused",
	"UEFI Capsule",
};

int do_hob(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
	EFI_PEI_HOB_POINTERS hob;
	UINT16 type;
	char * desc;
	int i = 0;

	hob.Raw = (UINT8 *)gd->arch.hob_list;

	printf("HOB list address: 0x%08x\n\n", (unsigned int)hob.Raw);

	printf(" No. | Address  | Type                 | Length in Bytes \n");
	printf("-----|----------|----------------------|-----------------\n");
	while (!END_OF_HOB_LIST(hob)) {
		printf(" %-3d | %08x |", i, (unsigned int)hob.Raw);
		type = hob.Header->HobType;
		if (type == EFI_HOB_TYPE_UNUSED)
			desc = "*Unused*";
		else if (type == EFI_HOB_TYPE_END_OF_HOB_LIST)
			desc = "**END OF HOB**";
		else if (type >= HOB_TYPE_MIN && type <= HOB_TYPE_MAX)
			desc = hob_type[type];
		else
			desc = "!!!Invalid Type!!!";
		printf(" %-20s |", desc);
		printf(" %-15d \n", hob.Header->HobLength);
		hob.Raw = GET_NEXT_HOB(hob);
		i++;
	}

	return 0;
}

/* -------------------------------------------------------------------- */

U_BOOT_CMD(
	hob,	1,	1,	do_hob,
	"print FSP Hand-Off Block information",
	""
);
