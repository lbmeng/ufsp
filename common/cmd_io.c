/*
 * Copyright (c) 2012 The Chromium OS Authors.
 *
 * SPDX-License-Identifier:     GPL-2.0+
 */

/*
 * IO space access commands.
 */

#include <common.h>
#include <command.h>
#include <malloc.h>
#include <asm/io.h>

/*
 * IO Display
 *
 * Syntax:
 *	iod{.b, .w, .l} {addr} {len}
 */
int do_io_iod(cmd_tbl_t *cmdtp, int flag, int argc, char *const argv[])
{
	ulong addr, length, bytes;
	int size;
	char *buf;
	int i;

	if (argc < 2)
		return CMD_RET_USAGE;

	size = cmd_get_data_size(argv[0], 4);
	if (size < 0)
		return 1;

	addr = simple_strtoul(argv[1], NULL, 16);

	/* If another parameter, it is the length to display.
	 * Length is the number of objects, not number of bytes.
	 */
	length = (argc > 2) ? simple_strtoul(argv[2], NULL, 16) : 64;

	bytes = size * length;
	buf = malloc(bytes);
	if (!buf)
		return 1;

	for (i = 0; i < bytes; i += size)
	{
		if (size == 4)
			*(u32 *)&buf[i] = inl(addr + i);
		else if (size == 2)
			*(u16 *)&buf[i] = inw(addr + i);
		else
			*(u8 *)&buf[i] = inb(addr + i);
	}

	/* Print the lines. */
	print_buffer(addr, buf, size, length, 0);
	free(buf);

	return 0;
}

int do_io_iow(cmd_tbl_t *cmdtp, int flag, int argc, char *const argv[])
{
	ulong addr, size, val;

	if (argc != 3)
		return CMD_RET_USAGE;

	size = cmd_get_data_size(argv[0], 4);
	if (size < 0)
		return 1;

	addr = simple_strtoul(argv[1], NULL, 16);
	val = simple_strtoul(argv[2], NULL, 16);

	if (size == 4)
		outl((u32) val, addr);
	else if (size == 2)
		outw((u16) val, addr);
	else
		outb((u8) val, addr);

	return 0;
}

/**************************************************/
U_BOOT_CMD(iod, 3, 0, do_io_iod,
	   "IO space display", "[.b, .w, .l] address [# of objects]");

U_BOOT_CMD(iow, 3, 0, do_io_iow,
	   "IO space modify",
	   "[.b, .w, .l] address");
