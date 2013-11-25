/** @file int.c
 *
 * @brief IRQ handler (C half)
 *
 * @author Alvin Zheng <dongxuez@andrew.cmu.edu>
 *         Minghao Wang <minghaow@andrew.cmu.edu>
 *         Yining Yang <yiningy@andrew.cmu.edu>
 * @date   21 Nov, 2013 16:09
 */

#include <types.h>
#include <assert.h>
#include <device.h>
#include <arm/reg.h>
#include <arm/timer.h>
#include <arm/interrupt.h>

static uint32_t* old_irq;        // Old SWI handler start position
static uint32_t old_irq_ins[2];  // Old SWI handler first two instructions

void init_interrupt() {
    old_irq = install_handler(old_irq_ins, 1, irq_wrapper);
    if (old_irq == NULL) panic("Unrecognized IRQ Vector\n");
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
    if (reg_read(INT_ICIP_ADDR) & 0x4000000) {
        timer_handler();
        dev_update(get_millis());
    }
}
