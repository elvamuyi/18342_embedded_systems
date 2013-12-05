/** @file  time.S
 *
 * @brief  time sycall wrapper
 *
 * @author Alvin Zheng <dongxuez@andrew.cmu.edu>
 *         Minghao Wang <minghaow@andrew.cmu.edu>
 *         Yining Yang <yiningy@andrew.cmu.edu>
 * @date   20 Nov. 2013 18:53
 */

#include <asm.h>
#include <bits/swi.h>

    .file "time.S"

FUNC(time)
	swi     TIME_SWI
	mov     pc, lr
