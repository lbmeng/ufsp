/*
 * (C) Copyright 2014
 * Bin Meng, bmeng.cn@gmail.com.
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

/*
 * CPU microcode for Tunnel Creek
 * To be loaded by FSP TempRamInitEntry()
 */

unsigned int ucode_base[] = {
#include <asm/arch-queensbay/M0220661105.h>
};
