/** @file time.S
 *
 * @brief time sycall wrapper
 *
 * @author Kartik Subramanian <ksubrama@andrew.cmu.edu>
 * @date 2008-10-31
 */

#include <asm.h>
#include <bits/swi.h>

    .file "task_create.S"

FUNC(task_create)
    stmfd	sp!, {r1-r2}
	swi     CREATE_SWI
	ldr     r1, =errno
	rsbs	r2, r0, #0
	strgt	r2, [r1]
	movgt	r0, #-1
	ldmfd	sp!, {r1-r2}
	mov     pc, lr
