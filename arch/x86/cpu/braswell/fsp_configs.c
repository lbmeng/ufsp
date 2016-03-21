/*
 * Copyright (C) 2016, Bin Meng <bmeng.cn@gmail.com>
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#include <common.h>
#include <asm/fsp/fsp_support.h>

DECLARE_GLOBAL_DATA_PTR;

/* ALC262 Verb Table - 10EC0662 */
static const u32 verb_table_data[] = {
	0x00172000,
	0x00172100,
	0x00172200,
	0x00172300,
	/* Widget node 0x01 */
	0x0017ff00,
	0x0017ff00,
	0x0017ff00,
	0x0017ff00,
	/* Pin widget 0x14 - FRONT (Port-D) */
	0x01471c10,
	0x01471d40,
	0x01471e01,
	0x01471f01,
	/* Pin widget 0x16 - CEN/LFE (Port-G) */
	0x01671c12,
	0x01671d60,
	0x01671e01,
	0x01671f01,
	/* Pin widget 0x18 - MIC1 (Port-B) */
	0x01871c30,
	0x01871d90,
	0x01871ea1,
	0x01871f01,
	/* Pin widget 0x19 - MIC2 (Port-F) */
	0x01971c00,
	0x01971dc0,
	0x01971e04,
	0x01971f40,
	/* Pin widget 0x1A - LINE1 (Port-C) */
	0x01a71c3f,
	0x01a71d30,
	0x01a71e81,
	0x01a71f01,
	/* Pin widget 0x1B - LINE2 (Port-E) */
	0x01b71cf0,
	0x01b71d11,
	0x01b71e11,
	0x01b71f41,
	/* Pin widget 0x1C - CD-IN */
	0x01c71cf0,
	0x01c71d11,
	0x01c71e11,
	0x01c71f41,
	/* Pin widget 0x1D - BEEP-IN */
	0x01d71c29,
	0x01d71d36,
	0x01d71e26,
	0x01d71f41,
	/* Pin widget 0x1E - S/PDIF-OUT */
	0x01e71c20,
	0x01e71d11,
	0x01e71e45,
	0x01e71f01,
	/* Widget node 0x20 */
	0x02050004,
	0x02040001,
	0x02050004,
	0x02040001,
};

/*
 * This needs to be in ROM since if we put it in CAR, FSP init loses it when
 * it drops CAR.
 *
 * TODO(bmeng.cn@gmail.com): Move to device tree when FSP allows it
 *
 * VerbTable: (RealTek ALC262)
 * Revision ID = 0xFF, support all steps
 * Codec Verb Table For AZALIA
 * Codec Address: CAd value (0/1/2)
 * Codec Vendor: 0x10EC0662
 */
static const struct azalia_verb_table verb_table[] = {
	{
		{
			0x10ec0662,
			0x0000,
			0xff,
			0x01,
			0x0008,
			0x0002,
		},
		(u32 *)verb_table_data
	}
};

const struct azalia_config azalia_config = {
	.pme_enable = 1,
	.docking_supported = 1,
	.docking_attached = 0,
	.hdmi_codec_enable = 1,
	.azalia_v_ci_enable = 1,
	.reserved = 0,
	.verb_table_num = 1,
	.verb_table = (struct azalia_verb_table *)verb_table,
	.reset_wait_timer_ms = 300
};

/**
 * Override the FSP's configuration data.
 * If the device tree does not specify an integer setting, use the default
 * provided in Intel's Braswell release FSP/BraswellFsp.bsf file.
 */
void update_fsp_configs(struct fsp_config_data *config,
			struct fspinit_rtbuf *rt_buf)
{
	struct upd_region *fsp_upd = &config->fsp_upd;

	/* Initialize runtime buffer for fsp_init() */
	rt_buf->common.stack_top = config->common.stack_top - 32;
	rt_buf->common.boot_mode = config->common.boot_mode;
	rt_buf->common.upd_data = &config->fsp_upd;

	fsp_upd->silicon_upd.azalia_cfg_ptr =
		(struct azalia_config *)&azalia_config;

	/*
	 * For Braswell B0 stepping, disable_punit_pwr_config must be set to 1
	 * otherwise it just hangs in fsp_init().
	 */
	if (gd->arch.x86_mask == 2)
		fsp_upd->silicon_upd.disable_punit_pwr_config = 1;
}
