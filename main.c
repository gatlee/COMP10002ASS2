/* C based on data in given dictionary
 * Created by: Gatlee Kaw (994017)
 *
 * First Created: 2018-05-01
 * Last Modified: 2018-05-21
 *
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

/*=============== INCLUDES =============== */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>

/*=============== CONSTANT DEFINITIONS  =============== */
#define FN_IND      0
#define LN_IND      1
#define NN_IND      2
#define UNKNOWN        (-1)

#define NAME_MAX    30
#define DICT_MAX    100
#define DICT_END    '%'
#define NUM_PARAM    4
#define NUM_PROBS    3

#define FN_STR        "FIRST_NAME"
#define LN_STR        "LAST_NAME"
#define NN_STR        "NOT_NAME"
#define DIV_STR        "========================="
/*=============== TYPEDEFS  =============== */

typedef char word_t[NAME_MAX];
typedef int probList_t[NUM_PROBS];
typedef struct {
    word_t name;
    probList_t prob;
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
    probList_t prob;
};

typedef struct {
    node_t *head;
    node_t *foot;
} list_t;

typedef node_t sentEnt_t;

/*===============  FUNCTION DECLARATIONS  =============== */

int getWord(char W[], int limit);
int isValid (char c);
void assignDictEntry(dictEntry_t *entry, rawEntry_t raw);
void fillDict(dictEntry_t dict[], int* dictLen);

void printDictEntry(dictEntry_t dict[]);
void printList(list_t *list);

void printStage(int num);
int wordLen(word_t word);
double avWordLen(dictEntry_t dict[], int dict_len);
int compWords(const void* a, const void* b);
void printPossibleIdents(list_t *sentence);
void assignIdentsToSent(dictEntry_t dict [], int dictSize, list_t *sentence);

int elimBasedOnPrev(sentEnt_t *current, sentEnt_t *prev);
int elimBasedOnNext(sentEnt_t *current, sentEnt_t *next);
void evalProbsToHighest(list_t *sentence);

void incProb(probList_t probs, int index);
void decProb(probList_t probs, int index);
/*prob operations */

void copyProbs(probList_t dest, const probList_t src);
void setProbToNN(probList_t dest);
void pruneProbs(list_t *sentence);
int guaranteedIdent(sentEnt_t *sentEnt);

/*listops funcs (credits Alistair Moffat)*/
list_t *genSentenceList();
list_t *make_empty_list(void);
list_t *insert_at_foot(list_t *list, data_t value);
void free_list(list_t *list);


/*===============  MAIN  =============== */
int
main(int argc, char *argv[]) {
    dictEntry_t dict[DICT_MAX];
    int dictSize = 0;
    fillDict(dict, &dictSize);

    /*STAGE 1 */
    printStage(1);
    printDictEntry(dict);


    /*STAGE 2 */
    printStage(2);
    printf("Number of names: %d\n", dictSize);
    printf("Average number of characters per name: %.2f\n",
            avWordLen(dict, dictSize));
    printf("\n");

    /*STAGE 3 */
    printStage(3);
    list_t *sentence = genSentenceList();
    printList(sentence);

    /*STAGE 4 */
    printStage(4);
    assignIdentsToSent(dict, dictSize, sentence);

    printPossibleIdents(sentence);
    printf("\n");

    /*STAGE 5 */
    printStage(5);
    /*try to remove impossibilities*/
    pruneProbs(sentence);
    /*after that, just resort to highest*/
    evalProbsToHighest(sentence);
    printPossibleIdents(sentence);


    /*Finally free the list*/
    free_list(sentence);
    return 0;

}


/*increases probability at index only if probability is above 0 */
void
incProb(probList_t probs, int index) {
    if (probs[index] > 0) {
        probs[index] += 100;
    }
}

/*just eliminate the possibility fam*/
void
decProb(probList_t probs, int index) {
    probs[index] = 0;
}

/* Eliminates possibile current identities based on previous word
 * Only makes adjustments if previous or next's identities are guaranteed
 *
 * Returns: 1 if identity changed from unkown to known. 0 in all other cases
 *
 * */
int elimBasedOnPrev(sentEnt_t *current, sentEnt_t *prev) {
    int currIdentity = guaranteedIdent(current);
    int prevIdentity = guaranteedIdent(prev);

    /*current identity already known so no action required/no change*/
    if (currIdentity!=UNKNOWN) {
        return 0;
    }
    /*eliminate possible identities for curr based on prev identity if prev
      guaranteed*/
    if (prevIdentity == FN_IND) {
        incProb(current->prob, LN_IND);
        decProb(current->prob, FN_IND);
    }
    if (prevIdentity == LN_IND) {
        incProb(current->prob, NN_IND);
        decProb(current->prob, LN_IND);

    }
    if (prevIdentity == NN_IND) {
        decProb(current->prob, LN_IND);
    }

    /*update identity*/
    currIdentity = guaranteedIdent(current);
    if (currIdentity == UNKNOWN) {
        return 0;
    } else {
        /*finally a change in it's guaranteed identity! */
        return 1;
    }





}

/* Eliminates possibile current identities based on the word in front of it
 * Only makes adjustments if previous or next's identities are guaranteed
 *
 * Returns: 1 if identity changed from unkown to known. 0 in all other cases
 *
 * */
int elimBasedOnNext(sentEnt_t *current, sentEnt_t *next) {
    int nextIdentity = guaranteedIdent(next);
    int currIdentity = guaranteedIdent(current);

    /*current identity already known so no action required/no change*/
    if (currIdentity != UNKNOWN) {
        return 0;
    }

    /*eliminate possibilities again */
    /*heuristics here can be tweaked as needed*/
    if (nextIdentity == FN_IND) {
        incProb(current->prob, LN_IND);
        decProb(current->prob, FN_IND);
    }
    if (nextIdentity == LN_IND) {
        incProb(current->prob, FN_IND);
        decProb(current->prob, LN_IND);
        decProb(current->prob, NN_IND);

    }

    /*update identity*/
    currIdentity = guaranteedIdent(current);
    if (currIdentity == UNKNOWN) {
        return 0;
    } else {
        /*finally a change in it's guaranteed identity! */
        return 1;
    }
}



/* Reduces prob in each sentEnt_t based on the probabilites of the next and
 * previous values only if those values are guaranteed to be FN LN or NN
 * Only adjust values which don't yet have a guaranteed value
 * */
void pruneProbs(list_t *sentence) {
    sentEnt_t *current = sentence->head;
    sentEnt_t *prev = NULL;
    int changed;

    /*only keep going if something went from uncertain to certain. to avoid
     *infinitely looping*/
    while (changed) {
        changed = 0;
        current = sentence->head;

        while (current) {
            if (prev != NULL && guaranteedIdent(current) == UNKNOWN) {
                changed += elimBasedOnPrev(current, prev);

            }
            if (current->next != NULL && guaranteedIdent(current) == UNKNOWN) {
                changed += elimBasedOnNext(current, current->next);

            }

            prev = current;
            current = current->next;
        }
    }

}


/*given a sentence entry, returns index of it's guaranteed identity
 *returns UNKNOWN if it doesn't exist
 * */
int guaranteedIdent(sentEnt_t *sentEnt) {
    int i;
    int total=0;
    int index;
    /*count up number of probabilities*/
    for (i=0; i< NUM_PROBS; i++) {
        if ((sentEnt->prob)[i] > 0) {
            total++;
            index = i;
        }
    }

    if (total == 1) {
        return index;

    } else {
        return UNKNOWN;

    }
}


/*given a sentence, set all probList array values to 0 except for largest
 * value in list*/
void evalProbsToHighest(list_t *sentence) {


    sentEnt_t *current = sentence->head;
    int currHi=0;
    int i;
    int currHiInd = 0;

    /*for every entry in sentence set all probs to 0 except for the highest
     *valued one */

    while (current) {
        /*search for highest probability index*/
        currHi=0;
        for (i=0; i< NUM_PROBS; i++) {
            if ((current->prob)[i] > currHi) {
                currHi = (current->prob)[i];
                currHiInd = i;
            }
        }

        /*sets probability of smaller ones to 0 so it won't get printed*/
        for (i=0; i< NUM_PROBS; i++) {
            if (i!= currHiInd) {
                (current->prob)[i] = 0;
            } else {
                (current->prob)[i] = 100;

            }
        }
        current = current->next;
    }

}

/* Input: sentence, dict
 * Gives each sentence node its own copy of corresponding dict probability or
 * if it is not in dict. Give it 100% chance of being not a name.
 *
 * */
void
assignIdentsToSent(dictEntry_t dict [], int dictSize, list_t *sentence) {

    sentEnt_t *current = sentence->head;
    dictEntry_t *target;


    while (current) {
        target = bsearch((void*)current, dict, dictSize, sizeof(dictEntry_t),
                         compWords);

        if (target!=NULL) {
            copyProbs(current->prob, target->prob);
        } else {
            setProbToNN(current->prob);

        }
        current = current->next;
    }
}


/*setProbs to values*/
void setProbToNN(probList_t dest) {
    dest[FN_IND] = 0;
    dest[LN_IND] = 0;
    dest[NN_IND] = 100;
}


/*copy values of problist_t from src to dest*/
void copyProbs(probList_t dest, const probList_t src) {
    int i;
    for (i=0; i<NUM_PROBS; i++) {
        dest[i] = src[i];
    }

}



/* Prints word followed by possible identities of the name.
 * Doesn't print if probability is < 0
 *
 * */
void printPossibleIdents(list_t *sentence) {
    node_t *current = sentence->head;

    int i;
    int first = 1;
    int printLast = 1;
    char *probStrings[3] = {FN_STR, LN_STR, NN_STR};

    while (current) {

        printf("%-32s", current->data);
        /*assign target to pointer to corresponding dictEntry_t */
        first = 1;
        printLast = 1;
        /*goes through all and prints expected output*/

        for (i=0; i<NUM_PROBS; i++) {
            if ((current->prob[i] > 0) && (i != NN_IND || printLast)) {

                /*first item doesn't need a comma before it*/
                if (!first) {
                    printf(", ");
                }
                first = 0;
                printf("%s", probStrings[i]);
                printLast = 0;
            }
        }

        printf("\n");
        current = current->next;
    }

}


/*printList without left justifying and buffering 25 characters. Needed because
 *verify program doesn't like having whitespace after each word
 *in stage 3 output*/
void printList(list_t *list) {
    node_t *current = list->head;

    while (current) {
        printf("%s\n", current->data);
        current = current->next;
    }
    printf("\n");

}

/* given two node funcitons
 * returns negative if a < b, 0 if a = b, and +ve if a > b
 * */
int compWords(const void* a, const void* b) {
    node_t*  aNode = (node_t*) a;
    node_t*  bNode = (node_t*) b;

    return strcmp(aNode->data , bNode->data);
}


/*creates the sentence list and returns pointer to it*/
list_t
*genSentenceList() {
    list_t *sentence;
    sentence = make_empty_list();

    word_t inpWord;
    while (getWord(inpWord, NAME_MAX) != EOF) {
        sentence = insert_at_foot(sentence, inpWord);
    }


    return sentence;

}

/* frees list
 * Credits: Alistair Moffat (See top of file for more information)
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
 * Credits: Alistair Moffat (See top of file for more information)
 */
list_t
*make_empty_list(void) {
    list_t *list;
    list = (list_t*)malloc(sizeof(*list));
    assert(list!=NULL);
    list->head = list->foot = NULL;
    return list;
}

/*gives average word length*/
double avWordLen(dictEntry_t dict[], int dict_len) {
    int i = 0;
    double total = 0.0;
    for (i=0; i<dict_len; i++) {
        total += wordLen(dict[i].name);


    }
    return total/dict_len;

}

/*Prints Stage Output with DIV_STR on either side of 'Stage <num>'
 * */
void printStage(int num) {

    char numC = num+'0';
    printf("%s", DIV_STR);
    printf("Stage %c", numC);
    printf("%s\n", DIV_STR);

}


/*Given user input from getWord function, copy each of the inputted values
 * into dict struct to generate dict
 * */
void fillDict(dictEntry_t dict[], int* dictLen) {
    word_t currInpWord;
    rawEntry_t rawEntry;
    int rawEntryPos = 0;
    int currEntry=0;
    while (getWord(currInpWord,NAME_MAX)!= EOF &&  currInpWord[0] != DICT_END) {

        /*copies values from input to rawEntry array*/
        strcpy(rawEntry[rawEntryPos], currInpWord);


        /*if last item of entry, start filling next entry in dict*/
        if (rawEntryPos == 3) {
            /*finally actually dictEntry in dict proper values from raw entry*/
            assignDictEntry(&(dict[currEntry]), rawEntry);

            currEntry++;
            rawEntryPos = 0;
            (*dictLen)++;
        } else {
            rawEntryPos++;
        }


    }
}

/*Prints the first dict entry for output purposes*/
void printDictEntry(dictEntry_t dict[]) {
    printf("Name %d: %s\n",  0, (dict[0]).name);
    printf("Label probabilities: %d%% %d%% %d%%\n",
            (dict[0]).prob[0],(dict[0]).prob[1], (dict[0]).prob[2]);

    printf("\n");
}

/*given the raw entry (array of strings), assign to dictionary with integer
 * values for probability*/

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
