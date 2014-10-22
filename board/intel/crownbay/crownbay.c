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
#include <asm/io.h>
#include <pci.h>
#include <netdev.h>

#define SERIAL_DEV PNP_DEV(0x2e, 4)

extern void lpc47m_enable_serial(u16 dev, u16 iobase);

DECLARE_GLOBAL_DATA_PTR;

int board_early_init_f(void)
{
	lpc47m_enable_serial(SERIAL_DEV, UART0_BASE);
	return 0;
}

int board_early_init_r(void)
{
	/* CPU Speed to 600MHz */
	gd->cpu_clk = 600000000;

	/* Crystal is 33.000MHz */
	gd->bus_clk = 33000000;

	return 0;
}

int board_eth_init(bd_t *bis)
{
	return pci_eth_init(bis);
}

int board_final_cleanup(void)
{
	return 0;
}

int last_stage_init(void)
{
	u32 bc;

	/* unprotect the whole SPI flash */
	pci_read_config_dword (PCI_BDF (0, 31, 0), 0xd8, &bc);
	bc |= 0x1;
	pci_write_config_dword (PCI_BDF (0, 31, 0), 0xd8, bc);

	printf("Intel Crown Bay CRB\n");
	return 0;
}
