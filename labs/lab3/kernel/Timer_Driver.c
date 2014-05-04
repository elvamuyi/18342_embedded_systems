/** @file  Timer_Driver.c
 *
 * @brief  Initialize the timer, increase and retrieve the timer value
 *
 * @author Alvin Zheng <dongxuez@andrew.cmu.edu>
 *         Minghao Wang <minghaow@andrew.cmu.edu>
 *         Yining Yang <yiningy@andrew.cmu.edu>
 * @date   Tue, 29 Oct 2013 16:09
 */

#include <types.h>
#include <arm/reg.h>
#include <arm/timer.h>
#include <arm/interrupt.h>

// Timer resolution in millisecond
#define TIMER_RESOLUTION 10

// Elapsed time since kernel boots up in milliseconds
static size_t timer;

/**
 * void initTimer(void)
 *  - Setup OIER, OSSR, ICMR, ICLR to enable IRQ time match
 *  - Initialize OSMR and OSCR and timer value to start counting
 */
void initTimer(void)
{
    // Enable IRQ on the match between OSCR and OSMR0
    reg_set(OSTMR_OSSR_ADDR, 0xf);
    reg_set(OSTMR_OIER_ADDR, 0x1);
    reg_set(INT_ICMR_ADDR, 0x4000000);
    reg_clear(INT_ICLR_ADDR, 0x4000000);

    // Initialize OSCR and OSMR
    timer = 0;
    reg_write(OSTMR_OSMR_ADDR(0), OSTMR_FREQ / 1000 * TIMER_RESOLUTION);
    reg_write(OSTMR_OSCR_ADDR, 0x0);
}

/**
 * void freeTimer(void)
 *  - Restore the timer interrupt registers
 */
void freeTimer(void)
{
    reg_set(OSTMR_OSSR_ADDR, 0xf);
    reg_clear(OSTMR_OIER_ADDR, 0x1);
    reg_clear(INT_ICMR_ADDR, 0x4000000);
    reg_write(OSTMR_OSMR_ADDR(0), 0x0);
}

/**
 * void addTimer(void)
 *  - Increase the timer value every time the IRQ time match is executed
 */
void addTimer(void)
{
    timer++;
}

/**
 * size_t getTimer(void)
 *  - Get the current timer value in millisecond 
 *  - Multiply the timer value by TIMER_RESOLUTION
 * Return:
 *  - size_t getTimer: Current OS time
 */
size_t getTimer(void)
{
    return timer * TIMER_RESOLUTION;
}
