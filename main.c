/* A first C program
*/


#include <stdio.h>
#include <stdlib.h>
#include "listops.c"

int
main(int argc, char *argv[]) {
	int i;
	for(i = 0; i < 10; i++) {
		printf("Hello World %d\n", i);
	}
	return 0;
}

