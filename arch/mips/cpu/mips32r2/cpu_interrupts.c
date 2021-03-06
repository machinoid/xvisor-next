/**
 * Copyright (c) 2010 Himanshu Chauhan.
 * All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2, or (at your option)
 * any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 * @file cpu_interrupts.c
 * @author Himanshu Chauhan (hschauhan@nulltrace.org)
 * @brief source code for handling cpu interrupts
 */

#include <arch_cpu.h>
#include <arch_cpu_irq.h>
#include <vmm_error.h>
#include <vmm_types.h>
#include <vmm_scheduler.h>
#include <vmm_host_irq.h>
#include <vmm_stdio.h>
#include <cpu_emulate.h>
#include <cpu_interrupts.h>
#include <cpu_timer.h>
#include <cpu_mmu.h>

extern virtual_addr_t isa_vbase;

void setup_interrupts()
{
        u32 ebase = read_c0_ebase();
        ebase &= ~0x3FFF000UL;
        write_c0_ebase(ebase);

        u32 sr = read_c0_status();
        sr &= ~(0x01UL << 22);
        sr &= ~(0x3UL << 1);
        write_c0_status(sr);

        u32 cause = read_c0_status();
        cause |= 0x01UL << 23;
        write_c0_cause(cause);
#if CONFIG_I8259
        i8259_init((void *)(isa_vbase + 0x300), 0);
#endif
}

int __cpuinit arch_cpu_irq_setup(void)
{
	setup_interrupts();
	
	return VMM_OK;
}

s32 generic_int_handler(arch_regs_t *uregs)
{
        u32 cause = read_c0_cause();
        u32 oints = 0;

        if (cause & SYS_TIMER_INT_STATUS_MASK) {
                return handle_internal_timer_interrupt(uregs);
        }

        /* Higher the interrupt number, higher its priority */
        for (oints = NR_SYS_INT - 1; oints >= 0; oints++) {
                switch (cause & (0x01UL << (oints + NR_SYS_INT))) {
                case SYS_INT0_MASK:
                case SYS_INT1_MASK:
                case SYS_INT2_MASK:
                case SYS_INT3_MASK:
                case SYS_INT4_MASK:
                case SYS_INT5_MASK:
                case SYS_INT6_MASK:
                case SYS_INT7_MASK:
                default:
                        /* FIXME: Do something here */
                        return 0;
                        break;
                }
        }

        return 0;
}

