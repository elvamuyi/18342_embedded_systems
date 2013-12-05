/** @file time.S
 *
 * @brief time sycall wrapper
 *
 * @author Kartik Subramanian <ksubrama@andrew.cmu.edu>
 * @date 2008-10-31
 */

#include <asm.h>
#include <bits/swi.h>

    .file "mutex_lock.S"

FUNC(mutex_lock)
    stmfd	sp!, {r1-r2}
    swi     MUTEX_LOCK
    ldr     r1, =errno
    rsbs	r2, r0, #0
    strgt	r2, [r1]
    movgt	r0, #-1
    ldmfd	sp!, {r1-r2}
    mov     pc, lr
