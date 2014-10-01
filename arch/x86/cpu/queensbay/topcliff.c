/*
 * (C) Copyright 2014
 * Bin Meng, bmeng.cn@gmail.com.
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#include <common.h>
#include <malloc.h>
#include <pci.h>
#include <sdhci.h>

int cpu_mmc_init(bd_t *bis)
{
	struct sdhci_host *mmc_host;
	u32 iobase;
	int i;

	for (i = 0; i < 2; i++) {
		mmc_host = (struct sdhci_host *)malloc(sizeof(struct sdhci_host));
		if (!mmc_host) {
			printf("sdhci_host %d malloc fail!\n", i);
			return -1;
		}
		mmc_host->name = "Topcliff SDHCI";
		pci_read_config_dword (PCI_BDF (2, 4, i), PCI_BASE_ADDRESS_0, &iobase);
		mmc_host->ioaddr = (void *)iobase;
		mmc_host->quirks = 0;
		add_sdhci(mmc_host, 0, 0);
	}

	return 1;
}
