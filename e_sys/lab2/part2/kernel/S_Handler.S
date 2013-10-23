@ S_Handler.S: Customized SWI Handler
@
@ Author: Alvin Zheng <dongxuez@andrew.cmu.edu>
@         Minghao Wang <minghaow@andrew.cmu.edu>
@         Yining Yang <yiningy@andrw.cmu.edu>
@ Date:   2013/10/22  12:00pm

	.file	"S_Handler.S"
	.text

	.global	S_Handler
S_Handler:
    SUB     sp, sp, #4
    STMFD   sp!, {r0-r12, lr}
    MRS     r2, spsr
    STR     r2, [sp, #56]
    MOV     r1, sp
    LDR     r0, [lr, #-4]
    BIC     r0, r0, #0xff000000
    MOV     lr, pc
    LDR     pc, =C_SWI_Handler
    LDR     r2, [sp, #56]
    MSR     spsr, r2
    LDMFD   sp!, {r0-r12, lr}
    ADD     sp, sp, #4
    MOVS    pc, lr
