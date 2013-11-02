/** @file crt0.S
 *
 * @brief Execution startup routine
 *
 * @author Mike Kasick <mkasick@andrew.cmu.edu>
 * @date   Sun, 07 Oct 2007 01:51:29 -0400
 */

#include <asm.h>
#include <bits/swi.h>

	.file "crt0.S"

FUNC(_start)
    ldr	r0, [sp]
	add	r1, sp, #4
	bl	main
	swi	EXIT_SWI
