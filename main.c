/* COMP10002 Assessment 2
 * By Gatlee Kaw (994017) created 2018-05-01, Last modified 2018-05-06
 * Project which identifies and labels a sentence with first_name or
 * last_name based on data in given dictionary
 * Algorithms are fun!
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

#define FN_IND      0
#define LN_IND      1
#define NN_IND      2
#define NAME_MAX	30
#define DICT_MAX	100
#define DICT_END	'%'
#define NUM_PARAM	4
#define NUM_PROBS	3

typedef char word_t[NAME_MAX];

typedef struct {
	word_t name;
	int  prob[NUM_PROBS];
} dictEntry_t;

typedef word_t rawEntry_t[NUM_PARAM];

/* List ops Program(s) written by Alistair Moffat
 * Additional information at top of document
 */

typedef struct node node_t;
typedef word_t data_t;
struct node {
	data_t data;
	node_t *next;
};

typedef struct {
	node_t *head;
	node_t *foot;
} list_t;






/*function declarations */

int getWord(char W[], int limit);
int isValid (char c);
void assignDictEntry(dictEntry_t *entry, rawEntry_t raw);
void fillDict(dictEntry_t dict[], int* dictLen);
void printDictAll(dictEntry_t dict[], int* dictLen);
void printDictOne(dictEntry_t dict[]);
void printStage(int num);
int wordLen(word_t word);
double avWordLens(dictEntry_t dict[], int dict_len);
void printList(list_t *list);
int compWords(const void* a, const void* b);
void printPossible(dictEntry_t dict [], int dictSize, list_t *sentence);

/*listops*/
list_t *genSentenceList();
list_t *make_empty_list(void);
list_t *insert_at_foot(list_t *list, data_t value);
void free_list(list_t *list);

/*Debug function*/
void printEntry(rawEntry_t raw);

/***********MAIN**********/
int
main(int argc, char *argv[]) {
	dictEntry_t dict[DICT_MAX];
	int dictSize = 0;
	fillDict(dict, &dictSize);

	/*STAGE 1 */
	printStage(1);
	printDictOne(dict);


	/*STAGE 2 */
	printStage(2);
	printf("Number of names: %d\n", dictSize);
	printf("Average length: %.2f \n\n", avWordLens(dict, dictSize));

	/*STAGE 3 */
	printStage(3);
	int sentLen;
	list_t *sentence = genSentenceList();
	printList(sentence);

	/*STAGE 4 */
	printStage(4);
	printf("%d", compWords(sentence->head, sentence->head->next));
	printPossible(dict, dictSize, sentence);






	return 0;

}

/* assigns either FIRST_NAME LAST_NAME or NOT_NAME based whether value is non
 * zero
 * TODO: pass through sentence length
 * */
void printPossible(dictEntry_t dict [], int dictSize, list_t *sentence) {
	node_t *current = sentence->head;
	node_t *target;
	int first = 1;
	char *probStrings[3] = {"FIRST_NAME", "LAST_NAME", "NOT_NAME"};
		

	while (current) {

		//printf("%s", current->data);
		//		printf("sizeof(current) = %d", sizeof(node_t));
		target = bsearch((void*)current, dict, dictSize, sizeof(dictEntry_t), compWords);
		if (target!=NULL) {
			printf("%s", target->data);
		}

		current = current->next;
	}
	/*read from sentence in a looop*/
		/*search for each sentence in dict*/
		/**/


}

/* given two node funcitons
 * returns -ve if a < b, 0 if a = b, and +ve if a > b
 * */
int compWords(const void* a, const void* b) {
	node_t*  aNode = (node_t*) a;
	node_t*  bNode = (node_t*) b;
	
	return strcmp(aNode->data , bNode->data);
}



void printList(list_t *list) {

	node_t *current = list->head;
	while (current) {
		printf("%s\n", current->data);
		current = current->next;
	}

}

list_t
*genSentenceList() {
	list_t *sentence;
	sentence = make_empty_list();

	word_t inpWord;
	int length = 0;
	while (getWord(inpWord, NAME_MAX) != EOF) {
		sentence = insert_at_foot(sentence, inpWord);
		length++;
		/*TODO: make a legnth function*/
	}

	


	return sentence;

}

/* frees list
 * Credits: Alistair Moffat
 *
 */
void
free_list(list_t *list) {
	node_t *curr, *prev;
	assert(list!=NULL);
	curr = list->head;
	while (curr) {
		prev = curr;
		curr = curr->next;
		free(prev);
	}
	free(list);
}

/* inserts item at foot at list
 * Credits: Alistair Moffat
 *
 */
list_t
*insert_at_foot(list_t *list, data_t value) {
	node_t *new;
	new = (node_t*)malloc(sizeof(*new));
	assert(list!=NULL && new!=NULL);
	strcpy(new->data, value);

	new->next = NULL;
	if (list->foot==NULL) {
		/* this is the first insertion into the list */
		list->head = list->foot = new;
	} else {
		list->foot->next = new;
		list->foot = new;
	}
	return list;
}

/* makes empty list
 * Credits: Alistair Moffat
 *
 */
list_t
*make_empty_list(void) {
	list_t *list;
	list = (list_t*)malloc(sizeof(*list));
	assert(list!=NULL);
	list->head = list->foot = NULL;
	return list;
}

double avWordLens(dictEntry_t dict[], int dict_len) {
	int i = 0;
	double total = 0.0;
	for (i=0; i<dict_len; i++) {
		total += wordLen(dict[i].name);


	}
	return total/dict_len;

}

/*Prints Stage Output*/
void printStage(int num) {

	char numC = num+'0';
	printf("=========================Stage %c=========================\n", numC);


}
void fillDict(dictEntry_t dict[], int* dictLen) {
	word_t currInpWord;
	rawEntry_t rawEntry;
	int rawEntryPos = 0;
	int currEntry=0;
	while (getWord(currInpWord,NAME_MAX)!= EOF &&  currInpWord[0] != DICT_END) {
		strcpy(rawEntry[rawEntryPos], currInpWord);


		if (rawEntryPos == 3) {
			assignDictEntry(&(dict[currEntry]), rawEntry);

			currEntry++;
			rawEntryPos = 0;
			(*dictLen)++;
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

}
void printDictAll(dictEntry_t dict[], int* dictLen) {
	int i;

	for (i=0; i<*dictLen;i++) {
		printf("Name %d: %s\n",  i, (dict[i]).name);
		printf("Label Probabilities: %d%% %d%% %d%%\n",
                (dict[i]).prob[0],(dict[i]).prob[1], (dict[i]).prob[2]);
	}

}

void printDictOne(dictEntry_t dict[]) {
	int one = 1;
	printDictAll(dict, &one);
	printf("\n");
}

void printEntry(rawEntry_t raw) {
  int i;
	for (i=0; i<4; i++) {
		printf("%s", raw[i]);
	}
}

void assignDictEntry(dictEntry_t *entry, rawEntry_t raw) {
	strcpy(entry->name, raw[0]);
	(entry->prob)[0] = atoi(raw[1]);
	(entry->prob)[1] = atoi(raw[2]);
	(entry->prob)[2] = atoi(raw[3]);
}

int wordLen(word_t word) {
	int total = 0;
	while (*word) {
		total++;
		word++;

	}
	return total;
}





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

/* Returns true if alphanumeric or DICT_END ('%')
 * Ignores # because not needed
 */
int
isValid (char c) {
	return ((c <= '9' && c >= '0') || (c == DICT_END) || isalpha(c));
}
