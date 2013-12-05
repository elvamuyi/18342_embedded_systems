/** @file io.c
 *
 * @brief Kernel I/O syscall implementations
 *
 * @author Alvin Zheng <dongxuez@andrew.cmu.edu>
 *         Minghao Wang <minghaow@andrew.cmu.edu>
 *         Yining Yang <yiningy@andrew.cmu.edu>
 * @date   21 Nov, 2013 16:09
 */

#include <types.h>
#include <kernel.h>
#include <syscall.h>
#include <exports.h>

#include <bits/errno.h>
#include <bits/fileno.h>
#include <arm/physmem.h>

#define EOT_CHAR 0x04
#define DEL_CHAR 0x7f

/* Read count bytes (or less) from fd into the buffer buf. */
ssize_t read_syscall(int fd __attribute__((unused)), void *buf __attribute__((unused)), size_t count __attribute__((unused)))
{
    // Verify file descriptor
    if (fd != STDIN_FILENO)
        return -EBADF;
    // Verify buffer address 
    if (!valid_addr(buf, count, RAM_START_ADDR, U_BOOT))
        return -EFAULT;

    // Read routine
    size_t length = 0;
    while (length < count) {
        unsigned char c = getc();
        // EOT
        if (c == EOT_CHAR) break;
        // Backspace or delete
        else if (c == 8 || c == DEL_CHAR) {
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
    return (ssize_t) length;
}

/* Write count bytes to fd from the buffer buf. */
ssize_t write_syscall(int fd __attribute__((unused)), const void *buf __attribute__((unused)), size_t count __attribute__((unused)))
{
    // Verify file descriptor
    if (fd != STDOUT_FILENO)
        return -EBADF;
    // Verify buffer address 
    if (valid_addr(buf, count, FLASH_END_ADDR, RAM_START_ADDR))
        return -EFAULT;

    // Write routine
    size_t length;
    for (length = 0; length < count; length++) {
        unsigned char c = *((char *) buf + length);
        if (c != '\0') putc(c);
        else break;
    }
    return (ssize_t) length;	
}
