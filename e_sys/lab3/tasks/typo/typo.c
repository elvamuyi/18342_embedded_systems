/** @file typo.c
 *
 * @brief Echos characters back with timing data.
 *
 * @author Alvin Zheng <dongxuez@andrew.cmu.edu>
 *         Minghao Wang <minghaow@andrew.cmu.edu>
 *         Yining Yang <yiningy@andrew.cmu.edu>
 * @date   Tue, 29 Oct 2013 16:09
 */

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#define BLOCK_SIZE 255

int main(int argc, char** argv)
{
	int i, t;
	char buffer[BLOCK_SIZE];
	
	while (1) {		
		for (i = 0; i < BLOCK_SIZE; i++)
			buffer[i] = '\0';

		write(STDOUT_FILENO, "> ", 2);
		
		t = time();
		if (read(STDIN_FILENO, buffer, BLOCK_SIZE) < 0)
			exit(1);
		t = time() - t;
		
		if (write(STDOUT_FILENO, buffer, BLOCK_SIZE) < 0)
			exit(1);

		printf("\n%d.%ds\n", t/1000, t/100-(t/1000)*10);
	}

	return 0;
}
