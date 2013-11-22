/** @file hello.c
 *
 * @brief Prints out Hello world using the syscall interface.
 *
 * Links to libc.
 *
 * @author Kartik Subramanian <ksubrama@andrew.cmu.edu>
 * @date   2008-10-29
 */
#include <unistd.h>

const char hello[] = "Hello World\r\n";

int main(int argc, char** argv)
{
    int i;
    for (i = 1; i < argc; i++) {
        write(STDOUT_FILENO, argv[i], 20);
        write(STDOUT_FILENO, "\n", 1);
    }
    write(STDOUT_FILENO, hello, sizeof(hello) - 1);
    sleep(3000);
	return 0xbadbad;
}
