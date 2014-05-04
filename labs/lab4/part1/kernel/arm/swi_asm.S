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

	.file	"swi_asm.S"

FUNC(swi_wrapper)
    /* Save user registers */
    STMFD   sp!, {r0-r12, lr}
    MRS     ip, spsr
    STMFD   sp, {ip, sp, lr}^
    ADD     sp, sp, #-12

    /* Build SWI handler arguments */
    LDR     r0, [lr, #-4]
    BIC     r0, r0, #0xff000000
    ADD     r1, sp, #12
    
    /* Load U-Boot global data into r8 */
    LDR     ip, =global_data
    LDR     r8, [ip]

    /* Enter swi_handler in C */
    MOV     lr, pc
    LDR     pc, =swi_handler

    /* Restore cpsr_usr, sp_usr, lr_usr */
    LDMFD   sp, {ip, sp, lr}^
    ADD     sp, sp, #12
    MSR     spsr, ip
    
    /* Pop the obsolete r0 */
    ADD     sp, sp, #4

    /* Restore user registers */
    LDMFD   sp!, {r1-r12, pc}^
