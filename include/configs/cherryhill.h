/*
 * Copyright (C) 2016, Bin Meng <bmeng.cn@gmail.com>
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#ifndef __CONFIG_H
#define __CONFIG_H

#include <configs/x86-common.h>

#define CONFIG_SYS_MONITOR_LEN		(2 << 20)
#define CONFIG_ARCH_MISC_INIT

#define CONFIG_PCI_PNP

#define CONFIG_STD_DEVICES_SETTINGS	"stdin=serial,vga,usbkbd\0" \
					"stdout=serial,vga\0" \
					"stderr=serial,vga\0"

#define CONFIG_SCSI_DEV_LIST		\
	{PCI_VENDOR_ID_INTEL, PCI_DEVICE_ID_INTEL_BRASWELL_SATA}

#define CONFIG_MMC
#define CONFIG_SDHCI
#define CONFIG_GENERIC_MMC
#define CONFIG_MMC_SDMA
#define CONFIG_CMD_MMC

/* Environment configuration */
#define CONFIG_ENV_SECT_SIZE		0x10000
#define CONFIG_ENV_OFFSET		0x005f0000

/* Braswell does not have legacy Intel ICH6 GPIO controller */
#undef CONFIG_INTEL_ICH6_GPIO

#endif	/* __CONFIG_H */
