/** @file splat.c
 *
 * @brief Displays a spinning cursor.
 *
 * Links to libc.
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
