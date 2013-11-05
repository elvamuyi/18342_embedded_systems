/** @file hardestGame.c
 *
 * @brief Prints out Hello world using the syscall interface.
 *
 * Links to libc.
 *
 * @author Kartik Subramanian <ksubrama@andrew.cmu.edu>
 * @date   2008-10-29
 */
#include <unistd.h>
#include <stdio.h>

#define BLOCK_SIZE 255

int main(int argc, char** argv)
{
     while (1) {
	int i, j;
	char buffer[BLOCK_SIZE] = {0};

	// print game welcome information
	const char hello[] = "Stop your car in the parking slot!\r\n\n";
	printf("\r\n\n\n=====================Game====================\r\n");
	write(STDOUT_FILENO, hello, sizeof(hello) - 1);

	// count down
	for (i = 3; i > 0; i--) {
		printf(" ... %d ... ", i);
		putchar('\r');
		sleep(200);
	}

	// game start, the car starts to run
	for (i = 0; i < 17; i++) {
		j = 0;
		while (j < i) {
			printf(".");
			j++;
		}
		printf("[##D");
		j = j + 4;
		while (j < 20) {
			printf(".");
			j++;
		}

		// print shield
	        for (j = 0; j < 20; j++) {
                	putchar((char)87);
        	}
		putchar('\r');
		sleep(100);
	}
	// hard to write in loops, save time
	printf(".................[##WWWWWWWWWWWWWWWWWWW\r");
	sleep(100);
	printf("..................[#WWWWWWWWWWWWWWWWWWW\r");
	sleep(100);
	printf("...................[WWWWWWWWWWWWWWWWWWW\r");
	sleep(100);
	printf("....................WWWWWWWWWWWWWWWWWWW\r");
	
	// waiting for users input
	int startTime = time();
	read(STDIN_FILENO, buffer, BLOCK_SIZE);
	int endTime = time();
	int timeDifference = endTime - startTime;
	int position = timeDifference / 100;
	
	// print result
	if (position > 15) {
		printf("................................|...[#@|\r\n");
		printf("!!!Hey!!! You just drive through the wall!");
	}
	else {
		i = 0;
	  	if (1) {
			while (i < 20 + position) {
				if (i == 32) printf("|");
				else printf(".");
				i++;
			}
			printf("[##D");
			i = i + 4;
			while (i < 39) {
				if (i == 32) printf("|");
				else printf(".");
				i++;
			}
			printf("|\r");
		}
	}
	putchar('\n');

	// print scores
	printf("====================Result===================\r\n");
	if (position > 15)
		printf("= Score: 0! It is a good time to buy a new car.\r\n");
	else if (position == 14 )
		printf("= Score: 10! Perfect! You are an excellent driver!\r\n");
	else if (position == 13 || position == 15)
		printf("= Score: 9! Nice~ You are a good driver!\r\n");
	else if (position > 11)
		printf("= Score: %d. You can drive, but be carefully!\r\n", position-5);
	else if (position > 4) 
		printf("= Score: %d. You'd better get more practice!\r\n", position-5);
	else printf("= Dude, give it up, please.\r\n");
	printf("=============================================\r\n");
     }

     return 1;
}
