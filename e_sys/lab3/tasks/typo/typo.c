/** @file  typo.c
 *
 * @brief  Echos characters back with timing data.
 *
 * @author Alvin Zheng <dongxuez@andrew.cmu.edu>
 *         Minghao Wang <minghaow@andrew.cmu.edu>
 *         Yining Yang <yiningy@andrew.cmu.edu>
 * @date   Thu, 31 Oct 2013 21:09
 */
 
#include <unistd.h>
#include <stdio.h>

#define BLOCK_SIZE 64
#define SLEEP_TIME 1000

int main(int argc, char** argv)
{
    int i;
    for (i = 1; i < argc; i++) {
        write(STDOUT_FILENO, argv[i], BLOCK_SIZE);
        write(STDOUT_FILENO, "\n", 1);
    }
    
    printf("Current time is %ld\n", time());
    sleep(SLEEP_TIME);
    printf("After sleeping for %d:\n", SLEEP_TIME);
    printf("Current time is %ld\n", time());
    
	return 0;
}

