/** @file typo.c
 *
 * @brief Echos characters back with timing data.
 *
 * Links to libc.
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
		write(STDOUT_FILENO, "> ", 2);
		
		t = time();
		if (read(STDIN_FILENO, buffer, BLOCK_SIZE) < 0)
			exit(1);
		t = time() - t;
		
		if (write(STDOUT_FILENO, buffer, BLOCK_SIZE) < 0)
			exit(1);
		
		for (i = 0; i < BLOCK_SIZE; i++)
			buffer[i] = '\0';
		
		//printf("\n%d.%ds\n", t/10000, t/1000-(t/10000)*10);
		printf("\n%dms\n", t);
	}

	return 0;
}
