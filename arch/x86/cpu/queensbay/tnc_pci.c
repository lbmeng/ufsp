/*
 * (C) Copyright 2014
 * Bin Meng, bmeng.cn@gmail.com.
 *
 * Copyright (c) 2011 The Chromium OS Authors.
 * (C) Copyright 2008,2009
 * Graeme Russ, <graeme.russ@gmail.com>
 *
 * (C) Copyright 2002
 * Daniel Engström, Omicron Ceti AB, <daniel@omicron.se>
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#include <common.h>
#include <pci.h>
#include <asm/pci.h>
#include <asm/fsp/fsp_support.h>

static struct pci_controller tnc_hose;

void pci_init_board(void)
{
	EFI_STATUS status;

	tnc_hose.first_busno = 0;
	tnc_hose.last_busno = 0xff;

	pci_set_region(tnc_hose.regions + 0, CONFIG_SYS_PCI_MEM_BUS, CONFIG_SYS_PCI_MEM_PHYS,
		CONFIG_SYS_PCI_MEM_SIZE, PCI_REGION_MEM);
	pci_set_region(tnc_hose.regions + 1, CONFIG_SYS_PCI_IO_BUS, CONFIG_SYS_PCI_IO_PHYS,
		CONFIG_SYS_PCI_IO_SIZE, PCI_REGION_IO);
	tnc_hose.region_count = 2;

	pci_setup_type1(&tnc_hose);

	pci_register_hose(&tnc_hose);

	tnc_hose.last_busno = pci_hose_scan(&tnc_hose);

	/* call into FspNotify */
	printf("Calling into FSP (notify phase EnumInitPhaseAfterPciEnumeration): ");
	if ((status = FspNotifyWrapper(NULL, EnumInitPhaseAfterPciEnumeration)) != FSP_SUCCESS)
		printf("fail, error code %x\n", status);
	else
		printf("OK\n");
}
