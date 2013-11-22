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
#include <syscall.h>
#include <bits/swi.h>

#include <arm/reg.h>
#include <arm/psr.h>
#include <arm/timer.h>
#include <arm/physmem.h>
#include <arm/exception.h>
#include <arm/interrupt.h>

#define NULL ((void *) 0)

#define IRQ_STACK_ADDR 0xa1000000

extern uint32_t global_data;
extern unsigned kStackPtr;

int enter_user_mode(void);
int valid_addr(const void* start, size_t count, uintptr_t base, uintptr_t bound);

// Install the customized SWI/IRQ Handler
unsigned* Install_Handler(unsigned [], int);
// Customized SWI Handler (Assembly half)
extern int SWI_Handler(void);
// Customized SWI Handler (C half)
void C_SWI_Handler(unsigned, unsigned *);
// Customized IRQ Handler (Assembly half)
extern int IRQ_Handler(void);
// Customized IRQ Handler (C half)
void C_IRQ_Handler(void);

// Call user applications (C half)
int C_Call_UserApp(int, char*[]);
// Call user applications (Assembly half)
extern int Call_UserApp(int, unsigned *, unsigned);

// Syscalls
extern void exit(int);

#endif /* KERNEL_H */
