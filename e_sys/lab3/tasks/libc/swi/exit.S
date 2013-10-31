/** @file  exit.S
 *
 * @brief  exit sycall wrapper
 *
 * @author Alvin Zheng <dongxuez@andrew.cmu.edu>
 *         Minghao Wang <minghaow@andrew.cmu.edu>
 *         Yining Yang <yiningy@andrew.cmu.edu>
 * @date   Tue, 29 Oct 2013 15:38
 */

#include <asm.h>
#include <bits/swi.h>

	.file "exit.S"

FUNC(exit)
	swi	EXIT_SWI
