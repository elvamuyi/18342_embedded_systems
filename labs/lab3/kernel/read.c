/** @file  read.c
 *
 * @brief  Read syscall function
 *
 * @author Alvin Zheng <dongxuez@andrew.cmu.edu>
 *         Minghao Wang <minghaow@andrew.cmu.edu>
 *         Yining Yang <yiningy@andrew.cmu.edu>
 * @date   Tue, 29 Oct 2013 16:09
 */

#include <types.h>
#include <kernel.h>
#include <exports.h>
#include <bits/errno.h>
#include <bits/fileno.h>

ssize_t read(int fd, void *buf, size_t count)
{
    // Verify file descriptor
    if (fd != STDIN_FILENO)
        return -EBADF;
    // Verify buffer address 
    unsigned buf_val = (unsigned) buf;
    if (buf_val >= SDRAM_LOW) {
        if (buf_val + count >= U_Boot)
            return -EFAULT;
    }
    else if (buf_val + 1 >= FlashROM_LOW + 1) {
        if (buf_val + count > FlashROM_HIGH)
            return -EFAULT;
    } else
        return -EFAULT;

    // Enable IRQ to continue the timer
    asm volatile ("MSR cpsr_c, #0x53":::"cc");

    // Read routine
    size_t length = 0;
    while (length < count) {
        unsigned char c = getc();
        // EOT
        if (c == 4) break;
        // Backspace or delete
        else if (c == 8 || c == 127) {
            if (length > 0) {
                *((char *) buf + length - 1) = '\0';
                puts("\b \b");
                length--;
            }
        }
        // Newline or carriage return
        else if (c == 10 || c == 13) {
            *((char *) buf + length) = c;
            putc(c);
            length++;
            break;
        }
        // Regular character
        else if (c > 31 && c < 127) {
            *((char *) buf + length) = c;
            putc(c);
            length++;
        }
    }
    putc('\n');
    asm volatile ("MSR cpsr_c, #0xd3":::"cc");
    return (ssize_t) length;
}
