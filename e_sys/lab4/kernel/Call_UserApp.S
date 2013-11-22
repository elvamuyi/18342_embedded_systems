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
#include <config.h>
#include <bits/swi.h>

	.file	"Call_UserApp.S"

FUNC(Call_UserApp)
    STMFD   sp!, {r1-r12, lr}
    // Save kernel stack ptr into global variable
    LDR     ip, =kStackPtr
    STR     sp, [ip]
    // Initialize sp_irq
    MSR     cpsr_c, #0xd2
    MOV     sp, r2
    // Transfer to user mode
    MSR     cpsr_c, #0x50
    MOV     sp, r1
    MOV     lr, pc
    LDR     pc, =USR_START_ADDR
    SWI     EXIT_SWI
