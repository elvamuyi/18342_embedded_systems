/**@file   time.c
 *
 * @brief  Time syscall function
 *
 * @author Alvin Zheng <dongxuez@andrew.cmu.edu>
 *         Minghao Wang <minghaow@andrew.cmu.edu>
 *         Yining Yang <yiningy@andrew.cmu.edu>
 * @date   Thu, 31 Oct 2013 21:09
 */

#include <kernel.h>
#include <exports.h>
#include <types.h>

size_t time(void)
{
    return get_timer(0);
}
