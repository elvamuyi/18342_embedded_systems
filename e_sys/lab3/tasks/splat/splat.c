/** @file splat.c
 *
 * @brief Displays a spinning cursor.
 *
 * @author Alvin Zheng <dongxuez@andrew.cmu.edu>
 *         Minghao Wang <minghaow@andrew.cmu.edu>
 *         Yining Yang <yiningy@andrew.cmu.edu>
 * @date   Tue, 29 Oct 2013 16:09
 */

#include <unistd.h>
#include <stdio.h>

int main(int argc, char** argv)
{
	const char spinChr[] = "|/-\\";
	int i = 0;
	while (1) {
		printf("...........  ");
		putchar(spinChr[(i++) % 4]);
		printf("  ...........");
		putchar('\r');
		sleep(200);
	}
	
	return 0;
}
