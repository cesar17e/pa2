#ifndef WORD_PROCESSING_H
#define WORD_PROCESSING_H

// Function prototypes for word processing functions.
int lowercaseAndcheckforletter(char *word, int size);
int frontCharCheck(char c);
int lastCharCheck(char c);
void rewriteWord(char *word);

#endif 

/*
This is the header file for the processing of words given to us by my scanning function whicch reads words from it's buffer
and then sends it to these functions to ensure it is a valid function

*/
