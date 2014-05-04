/** @file  IRQ_Handler.S
 *
 * @brief  Customized IRQ Handler (Assembly half)
 *
 * @author Alvin Zheng <dongxuez@andrew.cmu.edu>
 *         Minghao Wang <minghaow@andrew.cmu.edu>
 *         Yining Yang <yiningy@andrew.cmu.edu>
 * @date   Tue, 29 Oct 2013 16:09
 */

#include <asm.h>

	.file	"IRQ_Handler.S"

FUNC(IRQ_Handler)
    STMFD   sp!, {r0-r12, lr}
    MOV     lr, pc
    LDR     pc, =C_IRQ_Handler
    LDMFD   sp!, {r0-r12, lr}
    SUB     lr, lr, #4
    MOVS    pc, lr
