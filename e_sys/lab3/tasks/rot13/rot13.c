/*
 * rot13.c: ROT13 cipher test application
 *
 * Authors: Alvin Zheng <dongxuez@andrew.cmu.edu>
 *          Minghao Wang <minghaow@andrew.cmu.edu>
 *          Yining Yang <yiningy@andrew.cmu.edu>
 * Date:    10/14/2013 11:19am
 */

#include <unistd.h>
#include <stdlib.h>

#define BLOCK_SIZE 255

char caesar(char c) {
    if ((c >= 'A' && c <= 'M') || (c >= 'a' && c <= 'm'))
        return c + 13;
    if ((c >= 'N' && c <= 'Z') || (c >= 'n' && c <= 'z'))
        return c - 13;
    return c;
}

int main(int argc, char *argv[]) {
    int i, count;
    char buffer[BLOCK_SIZE];
    // Print all the argv on the screen
    for (i = 1; i < argc; i++) {
        write(STDOUT_FILENO, argv[i], BLOCK_SIZE);
        write(STDOUT_FILENO, "\n", 1);
    }
    // Rot13 routine
    while (1) {
        if (read(STDIN_FILENO, buffer, BLOCK_SIZE) < 0)
            exit(1);
        // Exit if no bytes input
        char tmp = buffer[0];
        if (tmp == '\n' || tmp == '\r' || tmp == '\0')
            exit(0);
        for (count = 0; count < BLOCK_SIZE; count++) {
            tmp = buffer[count];
            if (tmp == '\r' || tmp == '\n' || tmp == '\0') {
                buffer[count] = '\n';
                break;
            }
            buffer[count] = caesar(tmp);
        }
        if (write(STDOUT_FILENO, buffer, count + 1) < 0)
            exit(1);
        // Flush the buffer
        for (i = 0; i < count; i++)
            buffer[i] = '\0';
    }
    return 0;
}
