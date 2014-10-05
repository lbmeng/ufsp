/*
 * (C) Copyright 2014
 * Bin Meng, bmeng.cn@gmail.com.
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#include <common.h>
#include <command.h>
#include <linux/compiler.h>
#include <malloc.h>
#include <asm/cpuid/prefix.h>
#include <asm/cpuid/cpuid.h>
#include <asm/cpuid/handlers.h>
#include <asm/cpuid/state.h>

/* avoid include <asm/cpu.h> for the cpuid conflicts */
extern int cpu_have_cpuid(void);

enum {
	DUMP_FORMAT_DEFAULT,
	DUMP_FORMAT_VMWARE,
	DUMP_FORMAT_XEN,
	DUMP_FORMAT_XEN_SXP,
	DUMP_FORMAT_ETALLEN,
	DUMP_FORMAT_NONE
};

struct {
	const char *name;
	int value;
} formats [] = {
	{ "default",  DUMP_FORMAT_DEFAULT },
	{ "vmware",   DUMP_FORMAT_VMWARE },
	{ "xen",      DUMP_FORMAT_XEN },
	{ "sxp",      DUMP_FORMAT_XEN_SXP },
	{ "etallen",  DUMP_FORMAT_ETALLEN },
	{ NULL,       0 }
};

static int dump_format = DUMP_FORMAT_DEFAULT;
static uint32_t scan_to = 0;
int ignore_vendor = 0;

static void run_cpuid(struct cpuid_state_t *state, int dump)
{
	uint32_t i, j;
	uint32_t r;
	struct cpu_regs_t cr_tmp, ignore[2];
	const struct cpuid_leaf_handler_index_t *h;


	/* Arbitrary leaf that's probably never ever used. */
	ZERO_REGS(&ignore[0]);
	ignore[0].eax = 0x5FFF0000;
	state->cpuid_call(&ignore[0], state);

	/* Another arbitrary leaf. On KVM, there are two invalid returns, and they're
	 * split by the 0x80000000 boundary.
	 */
	ZERO_REGS(&ignore[1]);
	ignore[1].eax = 0x8FFF0000;
	state->cpuid_call(&ignore[1], state);

	for (r = 0x00000000;; r += 0x00010000) {
		/* If we're not doing a dump, we don't need to scan ranges
		 * which we don't actually have special handlers for.
		 */
		if (!dump) {
			for (h = decode_handlers;
				 h->handler;
				 h++)
			{
				if ((h->leaf_id & 0xFFFF0000) == r)
					break;
			}
			if (!h->handler)
				goto invalid_leaf;
		}
		state->curmax = r;
		for (i = r; i <= (scan_to ? r + scan_to : state->curmax); i++) {

			/* If a particular range is unsupported, the processor can report
			 * a really wacky upper boundary. This is a quick sanity check,
			 * since it's very unlikely that any range would have more than
			 * 0xFFFF indices.
			 */
			if ((state->curmax & 0xFFFF0000) != (i & 0xFFFF0000))
				break;

			ZERO_REGS(&cr_tmp);

			/* ECX isn't populated here. It's the job of any leaf handler to
			 * re-call CPUID with the appropriate ECX values.
			 */
			cr_tmp.eax = i;
			state->cpuid_call(&cr_tmp, state);

			/* Typically, if the range is invalid, the CPU gives an obvious
			 * "bogus" result. We try to catch that here.
			 *
			 * We don't compare the last byte of EDX (size - 1) because on
			 * certain very broken OSes (i.e. Mac OS X) there are no APIs to
			 * force threads to be affinitized to one core. This makes the
			 * value of EDX a bit nondeterministic when CPUID is executed.
			 */
			for (j = 0; j < sizeof(ignore) / sizeof(struct cpu_regs_t); j++) {
				if (i == r && 0 == memcmp(&ignore[j], &cr_tmp, sizeof(struct cpu_regs_t) - 4))
					goto invalid_leaf;
			}

			for (h = dump ? dump_handlers : decode_handlers;
			     h->handler;
			     h++)
			{
				if (h->leaf_id == i)
					break;
			}

			if (h->handler)
				h->handler(&cr_tmp, state);
			else if (dump)
				state->cpuid_print(&cr_tmp, state, FALSE);
		}
invalid_leaf:

		/* Terminating condition.
		 * This is an awkward way to terminate the loop, but if we used
		 * r != 0xFFFF0000 as the terminating condition in the outer loop,
		 * then we would effectively skip probing of 0xFFFF0000. So we
		 * turn this into an awkward do/while+for combination.
		 */
		if (r == 0xFFFF0000)
			break;
	}
}

static int do_cpuid_display(int argc, char * const argv[])
{
	struct cpuid_state_t state;
	int cpu_start, cpu_end;
	int do_dump = 0;
	int c;
	const char *cmd;

	cmd = argv[0];
	if (strcmp(cmd, "dump") == 0)
		do_dump = 1;

	INIT_CPUID_STATE(&state);

	cpu_start = cpu_end = 0;

	if (do_dump) {
		switch(dump_format) {
		case DUMP_FORMAT_DEFAULT:
			state.cpuid_print = cpuid_dump_normal;
			break;
		case DUMP_FORMAT_VMWARE:
			state.cpuid_print = cpuid_dump_vmware;
			break;
		case DUMP_FORMAT_XEN:
			state.cpuid_print = cpuid_dump_xen;
			printf("cpuid = [\n");
			break;
		case DUMP_FORMAT_XEN_SXP:
			state.cpuid_print = cpuid_dump_xen_sxp;
			printf("(\n");
			break;
		case DUMP_FORMAT_ETALLEN:
			state.cpuid_print = cpuid_dump_etallen;
			break;
		}
	}

	for (c = cpu_start; c <= cpu_end; c++) {
		switch(dump_format) {
		case DUMP_FORMAT_DEFAULT:
		case DUMP_FORMAT_ETALLEN:
			printf("CPU %d:\n", c);
			break;
		}
		run_cpuid(&state, do_dump);
	}

	if (do_dump) {
		switch (dump_format) {
		case DUMP_FORMAT_XEN:
			printf("]\n");
			break;
		case DUMP_FORMAT_XEN_SXP:
			printf(")\n");
			break;
		}
	}

	FREE_CPUID_STATE(&state);

	return 0;
}

static int do_cpuid_format(int argc, char * const argv[])
{
	const char *cmd;
	int c;
	int ret = -1;

	if (argc == 1) {
		printf("Current dump format is '%s'\n", formats[dump_format].name);
		ret = 0;
		goto done;
	}
	else
		cmd = argv[1];

	for (c = 0; formats[c].name != NULL; c++) {
		if (strcmp(cmd, formats[c].name) == 0) {
			dump_format = formats[c].value;
			ret = 0;
			break;
		}
	}

	if (!formats[c].name)
		printf("Unrecognized format: '%s'\n", cmd);
	else 
		printf("Dump format is set to '%s'\n", cmd);

done:
	return ret;
}

static int _do_cpuid(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
	const char *cmd;
	int ret;

	/* need at least two arguments */
	if (argc < 2)
		goto usage;

	cmd = argv[1];
	--argc;
	++argv;

	if (strcmp(cmd, "format") == 0) {
		ret = do_cpuid_format(argc, argv);
		goto done;
	}
	else if (strcmp(cmd, "decode") == 0 || strcmp(cmd, "dump") == 0) {
		ret = do_cpuid_display(argc, argv);
		goto done;
	}
	else
		ret = -1;

done:
	if (ret != -1)
		return ret;

usage:
	return CMD_RET_USAGE;
}

static int do_cpuid(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
	int ret = 0;

	if (cpu_have_cpuid())
		ret = _do_cpuid(cmdtp, flag, argc, argv);
	else
		printf("This CPU does not have CPUID support\n");

	return ret;
}

/* -------------------------------------------------------------------- */

U_BOOT_CMD(
	cpuid,	3,	1,	do_cpuid,
	"A simple CPUID decoder/dumper for x86/x86_64",
	"decode            - print decoded cpuid info with human readable words\n"
	"cpuid dump              - print raw cpuid information in current dump format\n" 
	"cpuid format [format]   - set dump format to [format] if [format] is specified,\n"
	"                          or display current dump format if [format] omitted.\n"
	"                          [format] can be one of the following:\n"
	"                            default | vmware | xen | sxp | etallen"
);
