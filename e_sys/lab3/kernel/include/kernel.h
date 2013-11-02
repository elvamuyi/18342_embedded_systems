/*
 * kernel.h: Defines the memory layout
 *
 * Author: Alvin Zheng <dongxuez@andrew.cmu.edu>
 * Date:   2013/10/24  12:00pm
 */

#ifndef KERNEL_H
#define KERNEL_H

#define NULL ((void *) 0)

#define SWI_VECTOR 0x08
#define IRQ_VECTOR 0x18
#define SDRAM_LOW 0xa0000000
#define SDRAM_HIGH 0xa3ffffff
#define FlashROM_LOW 0x0
#define FlashROM_HIGH 0x00ffffff
#define U_Boot 0xa3f00000
#define USER_APP_ADDR 0xa0000000
#define USER_STACK_ADDR 0xa3000000

#endif /* KERNEL_H */
