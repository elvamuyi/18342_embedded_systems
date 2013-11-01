/**@file   sleep.c
 *
 * @brief  Sleep syscall function
 *
 * @author Alvin Zheng <dongxuez@andrew.cmu.edu>
 *         Minghao Wang <minghaow@andrew.cmu.edu>
 *         Yining Yang <yiningy@andrew.cmu.edu>
 * @date   Thu, 31 Oct 2013 21:09
 */

#include <kernel.h>
#include <exports.h>
#include <types.h>

void sleep(unsigned long millis)
{
    udelay(millis);
}
