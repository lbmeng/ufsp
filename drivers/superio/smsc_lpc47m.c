/*
 * (C) Copyright 2014
 * Bin Meng, bmeng.cn@gmail.com.
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#include <common.h>
#include <asm/io.h>
#include <asm/pnp_def.h>

static void pnp_enter_conf_state(u16 dev)
{
	u16 port = dev >> 8;
	outb(0x55, port);
}

static void pnp_exit_conf_state(u16 dev)
{
	u16 port = dev >> 8;
	outb(0xaa, port);
}

/*
 * Configure the base I/O port of the specified serial device and enable the
 * serial device.
 *
 * @param dev High 8 bits = Super I/O port, low 8 bits = logical device number.
 * @param iobase Processor I/O port address to assign to this serial device.
 */
void lpc47m_enable_serial(u16 dev, u16 iobase)
{
	pnp_enter_conf_state(dev);
	pnp_set_logical_device(dev);
	pnp_set_enable(dev, 0);
	pnp_set_iobase(dev, PNP_IDX_IO0, iobase);
	pnp_set_enable(dev, 1);
	pnp_exit_conf_state(dev);
}
