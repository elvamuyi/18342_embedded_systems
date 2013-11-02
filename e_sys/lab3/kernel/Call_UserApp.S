/** @file  Call_UserApp.S
 *
 * @brief  Call user application from kernel (Assembly half)
 *          - Switch to user mode with IRQ & FIQ masked
 *          - Setup a full descending user mode stack
 *          - Jump to a loaded user program
 *
 * @author Alvin Zheng <dongxuez@andrew.cmu.edu>
 *         Minghao Wang <minghaow@andrew.cmu.edu>
 *         Yining Yang <yiningy@andrew.cmu.edu>
 * @date   Tue, 29 Oct 2013 16:09
 */

#include <asm.h>
#include <kernel.h>
#include <bits/swi.h>

	.file	"Call_UserApp.S"

FUNC(Call_UserApp)
    STMFD   sp!, {r1-r12, lr}
    @ Save kernel stack ptr into global variable
    LDR     ip, =kStackPtr
    STR     sp, [ip]
    @ Save $r8 into global variable
    LDR     ip, =UBoot_R8
    STR     r8, [ip]
    MSR     cpsr_c, #0xd0
    MOV     sp, r1
    MOV     lr, pc
    LDR     pc, =USER_APP_ADDR
    SWI     EXIT_SWI    // Exit with UserApp return value
