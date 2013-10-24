/*
 * write.c: Write syscall function
 *
 * Author: Alvin Zheng <dongxuez@andrew.cmu.edu>
 *         Minghao Wang <minghaow@andrew.cmu.edu>
 *         Yining Yang <yiningy@andrew.cmu.edu>
 * Date:   2013/10/23  11:30am
 */

#include <exports.h>
#include <bits/types.h>
#include <bits/errno.h>
#include <bits/fileno.h>

#define U_Boot 0xa3f00000
#define SDRAM_LOW 0xa0000000
#define SDRAM_HIGH 0xa3ffffff
#define FlashROM_LOW 0x0
#define FlashROM_HIGH 0x00ffffff

ssize_t write(int fd, const void *buf, size_t count)
{
    // Verify file descriptor
    if (fd != STDOUT_FILENO)
        return -EBADF;
    // Verify buffer address 
    unsigned buf_val = (unsigned) buf;
    if (buf_val >= SDRAM_LOW) {
        if (buf_val + count >= U_Boot)
            return -EFAULT;
    }
    else if (buf_val >= FlashROM_LOW) {
        if (buf_val + count > FlashROM_HIGH)
            return -EFAULT;
    } else
        return -EFAULT;

    // Write routine
    int length;
    for (length = 0; length < count; length++) {
        unsigned char c = *((char *) buf + length);
        if (c != '\0') putc(c);
        else break;
    }
    return length;
}
