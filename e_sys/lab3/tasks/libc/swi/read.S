/** @file  read.S
 *
 * @brief  read sycall wrapper
 *
 * @author Alvin Zheng <dongxuez@andrew.cmu.edu>
 *         Minghao Wang <minghaow@andrew.cmu.edu>
 *         Yining Yang <yiningy@andrew.cmu.edu>
 * @date   Tue, 29 Oct 2013 15:39
 */

#include <asm.h>
#include <bits/swi.h>

	.file "read.S"

FUNC(read)
	stmfd	sp!, {r1-r12, lr}
	add	sp, sp, #52
	ldmfd	sp, {r4-r6}
	swi	READ_SWI
	ldr	r1, =errno
	rsbs	r2, r0, #0
	strgt	r2, [r1]
	movgt	r0, #-1
	sub	sp, sp, #52
	ldmfd	sp!, {r1-r12, lr}
	mov	pc, lr
