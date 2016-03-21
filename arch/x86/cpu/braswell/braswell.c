/*
 * Copyright (C) 2016, Bin Meng <bmeng.cn@gmail.com>
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#include <common.h>
#include <mmc.h>
#include <pci_ids.h>
#include <asm/mrccache.h>
#include <asm/post.h>

static struct pci_device_id mmc_supported[] = {
	{ PCI_VENDOR_ID_INTEL, PCI_DEVICE_ID_INTEL_BRASWELL_MMC },
	{ PCI_VENDOR_ID_INTEL, PCI_DEVICE_ID_INTEL_BRASWELL_SDIO },
	{ PCI_VENDOR_ID_INTEL, PCI_DEVICE_ID_INTEL_BRASWELL_SD },
	{},
};

int cpu_mmc_init(bd_t *bis)
{
	return pci_mmc_init("Braswell SDHCI", mmc_supported);
}

int arch_cpu_init(void)
{
	int ret;

	post_code(POST_CPU_INIT);

	ret = x86_cpu_init_f();
	if (ret)
		return ret;

	return 0;
}

int arch_misc_init(void)
{
#ifdef CONFIG_ENABLE_MRC_CACHE
	/*
	 * We intend not to check any return value here, as even MRC cache
	 * is not saved successfully, it is not a severe error that will
	 * prevent system from continuing to boot.
	 */
	mrccache_save();
#endif

	return 0;
}

int reserve_arch(void)
{
#ifdef CONFIG_ENABLE_MRC_CACHE
	return mrccache_reserve();
#else
	return 0;
#endif
}

void reset_cpu(ulong addr)
{
	/* cold reset */
	x86_full_reset();
}
