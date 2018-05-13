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
#include <assert.h>
/*#include "listops.c"i*/
/*TODO: Attribute */

#define FN_IND      0
#define LN_IND      1
#define NN_IND      2
#define NAME_MAX	30
#define DICT_MAX	100
#define DICT_END	'%'
#define NUM_PARAM	4

typedef char word_t[NAME_MAX];

typedef struct {
    word_t name;
    int  prob[3]; 
} dictEntry_t;

typedef word_t rawEntry_t[NUM_PARAM];


/* function declarations */

int getWord(char W[], int limit);
int isValid (char c);
void assignDictEntry(dictEntry_t *entry, rawEntry_t raw);
void fillDict(dictEntry_t dict[], int* dictLen);
void printEntry(rawEntry_t raw);


/***********MAIN**********/
int
main(int argc, char *argv[]) {
	dictEntry_t dict[DICT_MAX];
	int dictLen = 0;
	
	fillDict(dict, &dictLen);
	/*SECTION1*/


	return 0;

}

void fillDict(dictEntry_t dict[], int* dictLen) {
	word_t currInpWord;
	rawEntry_t rawEntry;
	int rawEntryPos = 0;
	int currEntry = 0;
	while (getWord(currInpWord,NAME_MAX)!= EOF) {
		strcpy(rawEntry[rawEntryPos], currInpWord);


		if (rawEntryPos == 3) {

			assignDictEntry(&(dict[currEntry]), rawEntry);
			currEntry++;
			rawEntryPos = 0;
		} else {
			rawEntryPos++;
		}

		 

	}
	/*
	int i;
	for (i=0; i<DICT_MAX;i++) {
		assignDictEntry(&(dict[i]), "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaa", 0, 1, 2);
		
	}
	*/
	int i;

	for (i=0; i<2;i++) {
		printf("%d %s %d %d %d\n",  i, (dict[i]).name,
			  (dict[i]).prob[0],(dict[i]).prob[1], (dict[i]).prob[2]);
		
	}

}

void printEntry(rawEntry_t raw) {
	int i;
	for (i=0; i<4; i++) {
		printf("%s", raw[i]);
	}
}
/*
*void assignDictEntry(dictEntry_t *entry, char* name, int a, int b, int c) {
*	strcpy(entry->name, name);
*	(entry->prob)[0] = a;
*	(entry->prob)[1] = b;
*	(entry->prob)[2] = c;
*
*}
*/
void assignDictEntry(dictEntry_t *entry, rawEntry_t raw) {
	strcpy(entry->name, raw[0]);
	(entry->prob)[0] = atoi(raw[1]);
	(entry->prob)[1] = atoi(raw[2]);
	(entry->prob)[2] = atoi(raw[3]);
} 


/*

*generateDict(void){

	
	char word[NAME_LEN]; 
	int wordIndex = 0;
	int dataInd = 0;	

	while (getword(word, 30) != EOF) {
		if (dataInd == 0) {
			;
			//Assignword
		}

		//Assign nums
		if (dataInd == 3) {
			//Reset	
			dataInd = 0;
			wordIndex++;

		} else {
			dataInd++;
		}
		

	}
	return dict;

}
*/






/* Extract a single word (including numbers) out of the standard input, of not
   more than limit characters. Argument array W must be
   limit+1 characters or bigger. 
   Credit: Alistair Moffat (See top of file for additional info)
   Modified by Gatlee Kaw (accepts words comprising of digits)

*/

int
getWord(char W[], int limit) {
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

/* Returns true if alphanumeric or DICT_END
 * Ignores # because not needed
 */
int
isValid (char c) {
	return ((c <= '9' && c >= '0') || (c == DICT_END) || isalpha(c));
}
