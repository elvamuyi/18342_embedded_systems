/*
 * write.c: Write syscall function
 *
 * Author: Alvin Zheng <dongxuez@andrew.cmu.edu>
 *         Minghao Wang <minghaow@andrew.cmu.edu>
 *         Yining Yang <yiningy@andrew.cmu.edu>
 * Date:   2013/10/23  11:30am
 */

#include <kernel.h>
#include <exports.h>
#include <bits/types.h>
#include <bits/errno.h>
#include <bits/fileno.h>

ssize_t write(int fd, const void *buf, size_t count)
{
    // Verify file descriptor
    if (fd != STDOUT_FILENO)
        return -EBADF;
    // Verify buffer address 
    unsigned buf_val = (unsigned) buf;
    if (buf_val < SDRAM_LOW || buf_val + count > SDRAM_HIGH)
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
