/** @file  SWI_Handler.S
 *
 * @brief  Customized SWI Handler (Assembly half)
 *
 * @author Alvin Zheng <dongxuez@andrew.cmu.edu>
 *         Minghao Wang <minghaow@andrew.cmu.edu>
 *         Yining Yang <yiningy@andrew.cmu.edu>
 * @date   Tue, 29 Oct 2013 16:09
 */

#include <asm.h>

	.file	"SWI_Handler.S"

FUNC(SWI_Handler)
    SUB     sp, sp, #4
    STMFD   sp!, {r0-r12, lr}
    MRS     r2, spsr
    STR     r2, [sp, #56]
    LDR     r0, [lr, #-4]
    BIC     r0, r0, #0xff000000
    MOV     r1, sp
    LDR     ip, =UBoot_R8       // Load U-Boot global data into r8
    LDR     r8, [ip]
    MOV     lr, pc
    LDR     pc, =C_SWI_Handler  // r0 saves the return value
    LDR     r2, [sp, #56]
    MSR     spsr, r2
    ADD     sp, sp, #4          // Pop the obsolete r0
    LDMFD   sp!, {r1-r12, lr}
    ADD     sp, sp, #4
    MOVS    pc, lr
