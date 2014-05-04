/** @file  time.S
 *
 * @brief  time sycall wrapper
 *
 * @author Alvin Zheng <dongxuez@andrew.cmu.edu>
 *         Minghao Wang <minghaow@andrew.cmu.edu>
 *         Yining Yang <yiningy@andrew.cmu.edu>
 * @date   Thu, 31 Oct 2013 18:53
 */

#include <asm.h>
#include <bits/swi.h>

	.file "time.S"

FUNC(time)
	stmfd	sp!, {r1-r12, lr}
	swi	TIME_SWI
	ldmfd	sp!, {r1-r12, lr}
	mov	pc, lr
