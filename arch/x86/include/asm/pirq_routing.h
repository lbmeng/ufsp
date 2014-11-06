/*
 * Copyright (C) 2014  Bin Meng <bmeng.cn@gmail.com>
 *
 * This file is adapted from coreboot src/arch/x86/include/arch/pirq_routing.h
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#ifndef ARCH_PIRQ_ROUTING_H
#define ARCH_PIRQ_ROUTING_H

/* This is the maximum number on interrupt entries that a PCI device may have.
 *   This is NOT the number of slots or devices in the system
 *   This is NOT the number of entries in the PIRQ table
 * This tells us that in the PIRQ table, we are going to have 4 link-bitmap
 * entries per PCI device
 * It is fixed at 4: INTA, INTB, INTC, and INTD
 * CAUTION: If you change this, pirq_routing will not work correctly*/
#define MAX_INTX_ENTRIES 4

#define PIRQ_SIGNATURE	(('$' << 0) + ('P' << 8) + ('I' << 16) + ('R' << 24))
#define PIRQ_VERSION 0x0100

struct irq_info {
	u8 bus, devfn;	    /* Bus, device and function */
	struct {
		u8 link;    /* IRQ line ID, chipset dependent, 0=not routed */
		u16 bitmap; /* Available IRQs */
	} __attribute__((packed)) irq[4];
	u8 slot;	    /* Slot number, 0=onboard */
	u8 rfu;
} __attribute__((packed));

#ifndef CONFIG_IRQ_SLOT_COUNT
#warning "IRQ_SLOT_COUNT is not defined in Kconfig. PIRQ won't work correctly."
#endif

struct irq_routing_table {
	u32 signature;		/* PIRQ_SIGNATURE should be here */
	u16 version;		/* PIRQ_VERSION */
	u16 size;		/* Table size in bytes */
	u8  rtr_bus, rtr_devfn;	/* Where the interrupt router lies */
	u16 exclusive_irqs;	/* IRQs devoted exclusively to PCI usage */
	u16 rtr_vendor, rtr_device;/* Vendor/device ID of interrupt router */
	u32 miniport_data;
	u8  rfu[11];
	u8  checksum;		/* Modulo 256 checksum must give zero */
	struct irq_info slots[CONFIG_IRQ_SLOT_COUNT];
} __attribute__((packed));

unsigned long write_pirq_routing_table(unsigned long addr, struct irq_routing_table *routing_table);

#endif /* ARCH_PIRQ_ROUTING_H */
