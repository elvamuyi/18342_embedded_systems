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

#define OSMR0_ADDR 0x40a00000
#define OSMR1_ADDR 0x40a00004
#define OSMR2_ADDR 0x40a00008
#define OSMR3_ADDR 0x40a0000c
#define OSCR_ADDR 0x40a00010
#define OSSR_ADDR 0x40a00014
#define OIER_ADDR 0x40a0001c
#define OS_TIMER_FREQ 3250000

#define ICIP_ADDR 0x40d00000
#define ICMR_ADDR 0x40d00004
#define ICLR_ADDR 0x40d00008
#define ICFP_ADDR 0x40d0000c

#define SDRAM_LOW 0xa0000000
#define SDRAM_HIGH 0xa3ffffff
#define FlashROM_LOW 0x0
#define FlashROM_HIGH 0x00ffffff
#define U_Boot 0xa3f00000

#define USER_APP_ADDR 0xa0000000
#define USER_STACK_ADDR 0xa3000000
#define IRQ_STACK_ADDR 0xa1000000

#endif /* KERNEL_H */
