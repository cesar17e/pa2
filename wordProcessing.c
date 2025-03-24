#include <ctype.h>
#include <string.h>
#include <stdio.h>
#include "wordProcessing.h"

/*
    Here we will check our new string and organize to lower case and rid of characters that arent allowed as defined by project
        !- Words A word is a sequence of non-whitespace characters containing at least one letter; not
        !-  Starting with (, [, {, ", or ’; and not ending with ), ], }, ", ’, ,, ., !, or ?.
        !-In rewrite word we will use memove and pointer arithmatic so we skip all the non valid chars 
        !KEY WORD- MUST CONTAIN ONE LETTER  
*/
int lowercaseAndcheckforletter(char *word, int size){
    int hasALetter = 0; //
    for(int i =0; i < size; i++){
        word[i] = tolower(word[i]);
        if (isalpha(word[i])) {
            hasALetter = 1;
        }
    }
    return hasALetter; //If it returns 1 it has a letter, 0 if not
}


int frontCharCheck(char c){ //Returns 1 if true 0 if false
    if (c == '(' || c == '[' || c == '{' || c == '\"' || c == '\''){  
        return 1;
    }else{
        return 0;
    }
}
int lastCharCheck(char c){ //Returns 1 if true 0 if false
    if (c == ')' || c == ']' || c == '}' || c == '\"' || c == '\'' || c == ',' || c == '.' || c == '!' || c == '?'){
        return 1;
    }else{
        return 0;
    }
}

void rewriteWord(char *word){
    if(word == NULL){  //
        perror("String is empty find out why, in rewriteword");
        return; 
    } 

    int len = strlen(word); //Stops at the first null
    //Find the new start to use with memcopy
    int start = 0;

    while (start < len && (frontCharCheck(word[start]) == 1)) {
        start++;
    }

    //Next we need the ending valid char
    int end = len - 1; //Gives us ending index
    while (end >= start && (lastCharCheck(word[end]) ==1)) {
        end--;
    }

    //Now we have to calculate the new length
    int newLen = end - start + 1; 
    if (newLen <= 0) { //An exmaple of this happening ((!!)) will become nothing so newline is 0
        // The word became empty or invalid
        word[0] = '\0';
        return;
    }

    //Now we have to copy the substring
    if (start > 0) {
        memmove(word, word + start, newLen); //Copying 
    }
    word[newLen] = '\0'; //Now word is ready to be moved onto the hash-table

    //Before leaving lets make it all lowercase so that HELLO == hello and check if it contains at least one letter!
    int checkForLetter = lowercaseAndcheckforletter(word, newLen); 
    if(checkForLetter == 0){
        word[0] = '\0';
        return;
    }
    return;
}
