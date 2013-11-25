/** @file int.c
 *
 * @brief IRQ handler (C half)
 *
 * @author Alvin Zheng <dongxuez@andrew.cmu.edu>
 *         Minghao Wang <minghaow@andrew.cmu.edu>
 *         Yining Yang <yiningy@andrew.cmu.edu>
 * @date   21 Nov, 2013 16:09
 */

/**
 * void swi_handler(unsigned, unsigned *)
 *  - SWI Handler (C half), called by swi_asm (Assembly half)
 *  - Call the corresponding syscall
 *  - Syscall's return value saved in $R0 (except exit that never returns)
 * Argumnet:
 *  - unsigned swi_num: SWI number
 *  - unsigned *regs: Register list pointer
 */

#include <types.h>
#include <arm/reg.h>
#include <arm/psr.h>
#include <arm/timer.h>
#include <arm/exception.h>
#include <arm/interrupt.h>

extern uint32_t* install_handler(uint32_t [], int, void *);

static uint32_t* old_irq;        // Old SWI handler start position
static uint32_t old_irq_ins[2];  // Old SWI handler first two instructions

void init_interrupt() {
    old_irq = install_handler(old_irq_ins, 1, IRQ_Handler);
}

void destroy_interrupt() {
    *old_irq = old_irq_ins[0];
    *(old_irq + 1) = old_irq_ins[1];
}

/**
 * void irq_handler(void)
 *  - IRQ Handler (C half), called by irq_wrapper
 *  - Handle OSMR0 timer interrupt only
 *  - Increase the timer variable in Timer_Driver
 */
void irq_handler()
{
    if (reg_read(INT_ICIP_ADDR) & 0x4000000)
        timer_handler();
}
