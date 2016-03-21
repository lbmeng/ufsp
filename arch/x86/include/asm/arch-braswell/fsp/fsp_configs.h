/*
 * Copyright (C) 2016, Bin Meng <bmeng.cn@gmail.com>
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#ifndef __FSP_CONFIGS_H__
#define __FSP_CONFIGS_H__

struct fsp_config_data {
	struct fsp_cfg_common	common;
	struct upd_region	fsp_upd;
};

struct fspinit_rtbuf {
	struct common_buf	common;	/* FSP common runtime data structure */
};

#endif /* __FSP_CONFIGS_H__ */
