/* A first C program
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
/*#include "listops.c"i*/
/*TODO: Attribute */

#define FN_IND      0
#define LN_IND      1
#define NN_IND      2
#define NAME_LEN    30

typedef struct {
    char name[NAME_LEN + 1];
    int prob[3]; 
} word_t;


/* function declarations */

void read_line(char *line, int max_len);

int
main(int argc, char *argv[]) {
    
    word_t word; 
    int lookWord = 0; 
    int wordIndex = 0;
    int c;

    
   
    while ((c = getchar()) != EOF) {
        if (c == '#') {
            lookWord = 1; 
        } else if (c == '\n') {
            lookWord = 0;
            word.name[wordIndex] = '\0'; 
            wordIndex = 0;
            printf("%s", word.name);

        } else if (lookWord == 1) {
            word.name[wordIndex++] = c;
        }
        else if (lookWord == 0) {
            printf("num "); 
        }

    }
        
	return 0;
}


void
read_line(char *line, int max_len) {
    int i = 0, c;
    while (((c = getchar()) != EOF) && (c != '\n') && (c != '\r')) {
        if (i < max_len) {
            line[i++] = c;
        } else {
            printf("Invalid input line, toooooooo long.\n");
            exit(0);
        }
    }
    line[i] = '\0';
}

