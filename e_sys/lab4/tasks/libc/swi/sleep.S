/** @file  sleep.S
 *
 * @brief  sleep sycall wrapper
 *
 * @author Alvin Zheng <dongxuez@andrew.cmu.edu>
 *         Minghao Wang <minghaow@andrew.cmu.edu>
 *         Yining Yang <yiningy@andrew.cmu.edu>
 * @date   20, Nov 2013 18:53
 */

#include <asm.h>
#include <bits/swi.h>

    .file "sleep.S"

FUNC(sleep)
	swi     SLEEP_SWI
	mov     pc, lr
