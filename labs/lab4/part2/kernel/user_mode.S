/** @file  user_mode.S
 *
 * @brief  Call user application from kernel (Assembly half)
 *          - Switch to user mode with FIQ masked
 *          - Jump to a loaded user program
 *
 * @author Alvin Zheng <dongxuez@andrew.cmu.edu>
 *         Minghao Wang <minghaow@andrew.cmu.edu>
 *         Yining Yang <yiningy@andrew.cmu.edu>
 * @date   21 Nov, 2013 16:09
 */

#include <asm.h>
#include <config.h>
#include <arm/psr.h>

	.file	"user_mode.S"

FUNC(enter_user_mode)
    /* Transfer to user mode */
    MSR     cpsr_c, #PSR_MODE_USR
    LDR     sp, =USR_STACK
    MOV     r0, #0
    MOV     r1, sp
    MOV     lr, pc
    LDR     pc, =LOAD_ADDR
