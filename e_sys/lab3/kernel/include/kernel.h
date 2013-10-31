/**
 * @file   kernel.h
 *
 * @brief  Defines memory layout address.
 *
 * @author Alvin Zheng <dongxuez@andrew.cmu.edu>
 *         Minghao Wang <minghaow@andrew.cmu.edu>
 *         Yining Yang <yiningy@andrew.cmu.edu>
 * @date   Tue, 29 Oct 2013 15:39
 */

#ifndef BITS_KERNEL_H
#define BITS_KERNEL_H

#define SWI_VECTOR 0x08
#define SDRAM_LOW 0xa0000000
#define SDRAM_HIGH 0xa3ffffff
#define FlashROM_LOW 0x0
#define FlashROM_HIGH 0x00ffffff
#define U_Boot 0xa3f00000
#define USER_APP_ADDR 0xa0000000
#define USER_STACK_ADDR 0xa3000000

#endif /* BITS_KERNEL_H */
