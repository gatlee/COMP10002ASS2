/* COMP10002 Assessment 2
 * By Gatlee Kaw (994017) created 2018-05-01, Last modified 2018-05-06
 * Project which identifies and labels a sentence with first_name or 
 * last_name based on data in given dictionary
*/

/* Additional Credit:
   =====================================================================
   Program(s) written by Alistair Moffat, as an example for the book
   "Programming, Problem Solving, and Abstraction with C", Pearson
   Custom Books, Sydney, Australia, 2002; revised edition 2012,
   ISBN 9781486010974.

   
   See http://people.eng.unimelb.edu.au/ammoffat/ppsaa/ for further
   information.

   Prepared December 2012 for the Revised Edition.
   ================================================================== */

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
#define DICT_MAX	100
#define DICT_END	'%'

typedef struct {
    char name[NAME_LEN + 1];
    int prob[3]; 
} word_t;

typedef word_t dict_t[DICT_MAX];


/* function declarations */

int getword(char W[], int limit);
int isValid (char c);
dict_t *generateDict(void);


/***********MAIN**********/
int
main(int argc, char *argv[]) {
	dict_t *dict = generateDict();
	printf("%s \n", dict[0]->name);
	printf("%s \n", dict[1]->name);
	printf("%d \n", dict[0]->prob[0]);
	printf("%d \n", dict[0]->prob[1]);
	return 0;
}


dict_t 
*generateDict(void){
	
	char word[NAME_LEN]; 
	int wordIndex = 0;
	int dataInd = 0;	
	dict_t *dict = (dict_t *)malloc(DICT_MAX * sizeof(word_t));

	while (getword(word, 30) != EOF) {
		if (dataInd == 0) {
			strcpy(dict[wordIndex]->name , word);
		} else if (dataInd == 1) {
			/*TODO Assign actual values*/
			dict[wordIndex]->prob[0] = 0;
			
		} else if (dataInd == 2) {
			dict[wordIndex]->prob[1] = 1;

		} else if (dataInd == 3) {
			dict[wordIndex]->prob[2] = 2;

		}
		

		dataInd++;
		wordIndex++;
	}
	return dict;

}

/*TODO*/	
int
strToInt(static char* num) {
	int out;
}


/* Extract a single word (including numbers) out of the standard input, of not
   more than limit characters. Argument array W must be
   limit+1 characters or bigger. 
   Credit: Alistair Moffat (See top of file for additional info)
   Modified by Gatlee Kaw (accepts words comprising of digits)

*/

int
getword(char W[], int limit) {
	int c, len=0;
	/* first, skip over any non alphanumerics */
	while ((c=getchar())!=EOF && (!isValid(c))) {
		/* do nothing more */
	}
	if (c==EOF) {
		return EOF;
	}
	/* ok, first character of next word has been found */
	W[len++] = c;
	while (len<limit && (c=getchar())!=EOF && (isValid(c) )) {
		/* another character to be stored */
		W[len++] = c;
	}
	/* now close off the string */
	W[len] = '\0';
	return 0;
}

/* Returns true if alphanumeric or DICT_END */
int
isValid (char c) {
	return ((c <= '9' && c >= '0') || (c == DICT_END) || isalpha(c));
}
