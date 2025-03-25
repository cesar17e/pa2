#include <stdio.h>
#include <string.h>
#include <ctype.h>

// Include your word-processing functions. If you have moved these into a header file (e.g., word_processing.h),
// then include that instead. For this example, I'll define the functions inline.

int lowercaseAndcheckforletter(char *word, int size) {
    int hasALetter = 0;
    for (int i = 0; i < size; i++){
        word[i] = tolower((unsigned char)word[i]);
        if (isalpha((unsigned char)word[i])) {
            hasALetter = 1;
        }
    }
    return hasALetter;
}

int frontCharCheck(char c) { // Returns 1 if c is an unwanted leading punctuation.
    return (c == '(' || c == '[' || c == '{' || c == '\"' || c == '\'');
}

int lastCharCheck(char c) { // Returns 1 if c is an unwanted trailing punctuation.
    return (c == ')' || c == ']' || c == '}' || c == '\"' || c == '\'' ||
            c == ',' || c == '.' || c == '!' || c == '?');
}

void rewriteWord(char *word) {
    if (word == NULL) {
        perror("String is empty; find out why in rewriteWord");
        return;
    }
    
    int len = strlen(word); // stops at the first null character
    int start = 0;
    // Skip unwanted leading punctuation.
    while (start < len && frontCharCheck(word[start])) {
        start++;
    }
    
    // Find last valid character.
    int end = len - 1;
    while (end >= start && lastCharCheck(word[end])) {
        end--;
    }
    
    int newLen = end - start + 1;
    if (newLen <= 0) {
        // If all characters are stripped, mark as empty.
        word[0] = '\0';
        return;
    }
    
    // Shift the valid substring to the front of the buffer.
    if (start > 0) {
        memmove(word, word + start, newLen);
    }
    word[newLen] = '\0'; // null-terminate the string.
    
    // Convert to lowercase and check for at least one letter.
    if (lowercaseAndcheckforletter(word, newLen) == 0) {
        word[0] = '\0';
        return;
    }
}

int main() {
    // Define a list of test words.
    char testWords[][100] = {
        "((!!))",         // should be empty invalid chars
        "HELLO",          // becomes "hello"
        "((Hello))",      // becomes "hello"
        "HeLLo",          // becomes "hello"
        "9a1",            // remains "9a1" contains a letter
        "9",              // becomes empty, no letter
        "\"Hello!\"",     // becomes "hello"
        "[Test]",         // becomes "test"
        "hello-world",    // stays "hello-world" if the dash isn't stripped
        "((hello??"       // becomes "hello" strips the (( and trailing ??
    };
    
    int numTests = sizeof(testWords) / sizeof(testWords[0]);
    
    for (int i = 0; i < numTests; i++) {
        // Copy the test word into a mutable buffer.
        char word[100];
        strcpy(word, testWords[i]);
        
        printf("Original: \"%s\"\n", word);
        rewriteWord(word);
        if (strlen(word) == 0)
            printf("Rewritten: <empty>\n\n");
        else
            printf("Rewritten: \"%s\"\n\n", word);
    }
    
    return 0;
}
