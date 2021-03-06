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
 * @file cpu_locks.c
 * @author Himanshu Chauhan (hchauhan@nulltrace.org)
 * @brief Architecture specific implementation of synchronization mechanisms.
 */

#include <vmm_error.h>
#include <vmm_types.h>

void __lock __cpu_spin_lock(spinlock_t *lock)
{
        int tmp;
	u32 *lcounter = (u32 *)&lock->__cpu_lock.counter;

	__asm__ __volatile__ (
		"1: ll %0, 0(%2)\n\t"
		"bgtz %0, 1b\n\t"
		"nop\n\t"
		"addiu %0,%0,1\n\t"
		"sc %0,0(%2)\n\t"
		"beq %0, $0, 1b\n\t"
		:"=&r"(tmp), "+m"(lcounter)
		:"r"(lcounter));
}

void __lock __cpu_spin_unlock(spinlock_t *lock)
{
	int tmp;
	u32 *lcounter = (u32 *)&lock->__cpu_lock.counter;

	__asm__ __volatile__ (
		"1: ll %0, 0(%2)\n\t"
		"2:\n\t"
		"beq %0, $0, 2b\n\t"
		"nop\n\t"
		"addiu %0, %0, -1\n\t"
		"sc %0, 0(%2)\n\t"
		"beq %0, $0, 1b\n\t"
		:"=&r"(tmp), "+m"(lcounter)
		:"r"(lcounter));
}

bool __lock vmm_spin_lock_check(spinlock_t * lock)
{
	return (lock->__cpu_lock.counter) ? TRUE : FALSE;
}

void __lock vmm_spin_lock (spinlock_t *lock)
{
	__cpu_spin_lock(lock);
}

void __lock vmm_spin_unlock (spinlock_t *lock)
{
	__cpu_spin_unlock(lock);
}

