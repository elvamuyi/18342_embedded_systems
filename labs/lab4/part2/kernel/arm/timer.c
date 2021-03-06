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
#include <config.h>
#include <arm/reg.h>
#include <arm/timer.h>
#include <arm/interrupt.h>

// OSSR match value
#define OSTMR_OSSR_MATCH  (OSTMR_FREQ / 1000 * OS_TIMER_RESOLUTION)

// Elapsed time since kernel boots up in milliseconds
static volatile size_t timer;

/**
 * void init_timer(void)
 *  - Setup OIER, OSSR, ICMR, ICLR to enable IRQ time match
 *  - Initialize OSMR and OSCR and timer value to start counting
 */
void init_timer(void)
{
    // Enable IRQ on the match between OSCR and OSMR0
    reg_set(OSTMR_OSSR_ADDR, OSTMR_OSSR_M0);
    reg_set(OSTMR_OIER_ADDR, OSTMR_OIER_E0);
    reg_set(INT_ICMR_ADDR, (1 << INT_OSTMR_0));
    reg_clear(INT_ICLR_ADDR, (1 << INT_OSTMR_0));

    // Initialize OSCR and OSMR
    timer = 0;
    reg_write(OSTMR_OSMR_ADDR(0), OSTMR_OSSR_MATCH);
    reg_write(OSTMR_OSCR_ADDR, 0x0);
}

/**
 * void destroy_timer(void)
 *  - Restore the timer interrupt registers
 */
void destroy_timer(void)
{
    reg_set(OSTMR_OSSR_ADDR, OSTMR_OSSR_M0);
    reg_clear(OSTMR_OIER_ADDR, OSTMR_OIER_E0);
    reg_clear(INT_ICMR_ADDR, (1 << INT_OSTMR_0));
    reg_write(OSTMR_OSMR_ADDR(0), 0x0);
}

/**
 * void timer_handler(void)
 *  - Increase the timer value every time the IRQ time match is executed
 *  - Clear the INT and reset OSCR for the next cycle
 */
void timer_handler(void)
{
    // Increase the timer and clear the interrupts
    timer++;
    reg_set(OSTMR_OSSR_ADDR, OSTMR_OSSR_M0);

    // Write the next match value
    size_t next_match = get_ticks() + OSTMR_OSSR_MATCH;
    reg_write(OSTMR_OSMR_ADDR(0), next_match);
}

/**
 * size_t get_millis(void)
 *  - Get the current ticks 
 * Return:
 *  - size_t: Current OCCR value
 */
size_t get_ticks(void)
{
    return reg_read(OSTMR_OSCR_ADDR);
}

/**
 * size_t get_millis(void)
 *  - Get the current timer value in millisecond 
 *  - Multiply the timer value by TIMER_RESOLUTION
 * Return:
 *  - size_t: Current OS time
 */
size_t get_millis(void)
{
    return timer * OS_TIMER_RESOLUTION;
}
