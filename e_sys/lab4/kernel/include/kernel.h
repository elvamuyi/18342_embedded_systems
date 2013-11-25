/** @file kernel.h
 *
 * @brief Main kernel -- primary interface.
 *
 * @author Kartik Subramanian <ksubrama@andrew.cmu.edu>
 * @date   2008-11-17
 */

#ifndef KERNEL_H
#define KERNEL_H

#include <types.h>
#include <config.h>

#include <arm/swi.h>
#include <arm/timer.h>
#include <arm/physmem.h>
#include <arm/interrupt.h>

#define IRQ_STACK_ADDR 0xa1000000

extern uint32_t global_data;
extern unsigned kStackPtr;

int valid_addr(const void* start, size_t count, uintptr_t base, uintptr_t bound);

// Install the customized SWI/IRQ Handler
// Customized SWI Handler (Assembly half)
// Customized SWI Handler (C half)
// Customized IRQ Handler (Assembly half)
// Customized IRQ Handler (C half)

// Call user applications (C half)
int c_enter_user_mode(int, char*[]);
// Call user applications (Assembly half)
extern int enter_user_mode(int, unsigned *, unsigned);

#endif /* KERNEL_H */
