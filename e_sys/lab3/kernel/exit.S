/** @file  exit.S
 *
 * @brief  Exit syscall function
 *
 * @author Alvin Zheng <dongxuez@andrew.cmu.edu>
 *         Minghao Wang <minghaow@andrew.cmu.edu>
 *         Yining Yang <yiningy@andrew.cmu.edu>
 * @date   Tue, 29 Oct 2013 16:09
 */

#include <asm.h>
#include <bits/swi.h>

	.file	"exit.S"

FUNC(exit)
    LDR     r12, =kStackPtr
    LDR     sp, [r12]
    LDMFD   sp!, {r1-r12, pc}
