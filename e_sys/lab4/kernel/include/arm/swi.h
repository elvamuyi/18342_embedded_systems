/**
 * @file swi.h
 *
 * @brief Definitions for the software interrupt handler.
 *
 * @author Kartik Subramanian <ksubrama@andrew.cmu.edu>
 *
 * @date 2008-11-18
 */

#ifndef _SWI_H_
#define _SWI_H_

#ifndef ASSEMBLER

void init_swi(void);
void destroy_swi(void);
void swi_handler(unsigned, unsigned *);
extern void swi_wrapper(void);

#endif /* ASSEMBLER */

#endif /* _SWI_H_ */
