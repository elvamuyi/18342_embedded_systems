/** @file kernel.h
 *
 * @brief Main kernel -- primary interface.
 *
 * @author Kartik Subramanian <ksubrama@andrew.cmu.edu>
 * @date   2008-11-17
 */

#ifndef KERNEL_H
#define KERNEL_H

#include <lock.h>
#include <types.h>
#include <device.h>
#include <assert.h>

#include <arm/swi.h>
#include <arm/timer.h>
#include <arm/interrupt.h>

extern uint32_t global_data;
extern int enter_user_mode(void);
int valid_addr(const void* start, size_t count, uintptr_t base, uintptr_t bound);

#endif /* KERNEL_H */
