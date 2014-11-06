/*
 * Copyright (C) 2014  Bin Meng <bmeng.cn@gmail.com>
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#ifndef BOOT_TABLES_H
#define BOOT_TABLES_H

#define ROM_TABLE_LOW	0xf0000
#define ROM_TABLE_HIGH	0x100000

void write_tables(void);

#endif /* BOOT_TABLES_H */
