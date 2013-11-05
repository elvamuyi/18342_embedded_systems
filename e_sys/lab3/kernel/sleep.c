/**@file   sleep.c
 *
 * @brief  Sleep syscall function
 *
 * @author Alvin Zheng <dongxuez@andrew.cmu.edu>
 *         Minghao Wang <minghaow@andrew.cmu.edu>
 *         Yining Yang <yiningy@andrew.cmu.edu>
 * @date   Thu, 31 Oct 2013 21:09
 */

#include <types.h>

extern size_t getTimer(void);

void sleep(size_t sleep_time)
{
    size_t wait_time = getTimer() + sleep_time;
    asm volatile ("MSR cpsr_c, #0x53");
    while (getTimer() < wait_time);
    asm volatile ("MSR cpsr_c, #0xd3");
}
