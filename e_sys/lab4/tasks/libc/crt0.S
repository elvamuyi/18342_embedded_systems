/** @file crt0.S
 *
 * @brief Execution startup routine
 *
 * @author Alvin Zheng <dongxuez@andrew.cmu.edu>
 * @       Minghao Wang <minghaow@andrew.cmu.edu>
 * @       Yining Yang <yiningy@andrew.cmu.edu>
 * @date   2013/11/20
 */

#include <asm.h>
#include <bits/swi.h>

	.file "crt0.S"

FUNC(_start)
	ldr	r0, [sp]
	add	r1, sp, #4
	bl	main
	swi	EXIT_SWI
