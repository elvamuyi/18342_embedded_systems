/** @file time.c
 *
 * @brief Kernel timer based syscall implementations
 *
 * @author Alvin Zheng <dongxuez@andrew.cmu.edu>
 *         Minghao Wang <minghaow@andrew.cmu.edu>
 *         Yining Yang <yiningy@andrew.cmu.edu>
 * @date   21 Nov, 2013 21:09
 */

#include <types.h>
#include <config.h>
#include <syscall.h>
#include <bits/errno.h>

#include <arm/timer.h>

size_t time_syscall(void)
{
    return get_millis();
}

void sleep_syscall(unsigned long millis __attribute__((unused)))
{
    size_t wait_time = get_millis() + millis;
    while (get_millis() < wait_time);
}
