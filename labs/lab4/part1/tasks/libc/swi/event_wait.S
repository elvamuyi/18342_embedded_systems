/** @file event_wait.S
 *
 * @brief time sycall wrapper
 *
 * @author Kartik Subramanian <ksubrama@andrew.cmu.edu>
 * @date 2008-10-31
 */

#include <asm.h>
#include <bits/swi.h>

    .file "event_wait.S"

FUNC(event_wait)
    stmfd	sp!, {r1-r2}
    swi     EVENT_WAIT
    ldr     r1, =errno
    rsbs	r2, r0, #0
    strgt	r2, [r1]
    movgt	r0, #-1
    ldmfd	sp!, {r1-r2}
    mov     pc, lr
