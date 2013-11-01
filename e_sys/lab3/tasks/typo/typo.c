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
	char buf[BLOCK_SIZE];
    int r_val = 0;
    int w_val = 0;
    while((r_val!=-1) && (w_val!=-1))
    {
        r_val = read(STDIN_FILENO, buf, BLOCK_SIZE);
            
        int i;
        for(i = 0; i < BLOCK_SIZE; i++)
            if(buf[i] == '\n') break;
        
        w_val = write(STDOUT_FILENO, buf, i+1);
    }
    
    printf("\nCurrent time is %ld\n", time());
    sleep(SLEEP_TIME);
    printf("After sleeping for %d:", SLEEP_TIME);
    printf("\nCurrent time is %ld\n", time());
    
	return 0;
}

