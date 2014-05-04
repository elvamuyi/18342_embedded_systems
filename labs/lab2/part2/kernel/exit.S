@ exit.S: Exit syscall function
@
@ Author: Alvin Zheng <dongxuez@andrew.cmu.edu>
@         Minghao Wang <minghaow@andrew.cmu.edu>
@         Yining Yang <yiningy@andrw.cmu.edu>
@ Date:   2013/10/25  12:00pm

#include <bits/swi.h>

	.file	"exit.S"
	.text

	.global	exit
exit:
    LDR     r12, =kStackPtr
    LDR     sp, [r12]
    LDMFD   sp!, {r1-r12, pc}
