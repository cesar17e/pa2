#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <dirent.h>
#include <unistd.h>
#include <ctype.h>

#define MAXSIZE 8192// Buffer size

/*
    Purpose:
        This function opens a file given by its filename, reads its contents in chunks,
        and extracts words from the file. A word is defined as a sequence of non-whitespace
        characters. When a word is complete the function prints out the word along with its order of occurence
    Test:
        We will create a text file containing multiple words with spacing, newlines, and punctuation. 
        For example, a file with:
            "Hello World\nThis is   a test."
        Expect the function to print each extracted word in order.
    Expectation:
        - It should correctly detect word boundaries using isspace()
        - It should print each word along with its index.
        - If the file ends without whitespace, the last word should still be processed.
        - It should finally print the total number of words processed.
*/

void scanningFiles(const char *filename){ 
    // Open the file in read-only mode.
    int fd = open(filename, O_RDONLY); // Returns a unique file descriptor, or -1 on error.
    if (fd == -1) {
        perror("Open failed on scanning files");
        return;
    }  
    printf("Opened %s", filename);
    
    char buffer[MAXSIZE+1];  // Buffer to hold data read from file.
    int bytesRead;
    int wordCounter = 0;     // Counts the number of words processed.
    char wordArray[wordArraySize]; // Buffer to build each word.
    int wordIndex = 0;       // Index within wordArray.
    int insideAWord = 0;     // Flag: 1 if we are currently reading a word, 0 otherwise.

    // Read the file in chunks of size MAXSIZE.
    while ((bytesRead = read(fd, buffer, MAXSIZE)) > 0) {
        // Process each character in the current buffer.
        for (int i = 0; i < bytesRead; i++) {
            char c = buffer[i];
            // Check if the character is whitespace or a null terminator.
            if (isspace(c) || c == '\0' ) {
                // If we were in the middle of reading a word, then we have reached the end of the word.
                if (insideAWord == 1) {
                    wordArray[wordIndex] = '\0'; // Terminate the word.
                    wordIndex = 0;              // Reset the word buffer index.
                    wordCounter++;              // Increment the word counter.
                    // Print the word with its index.
                    printf("Index %d, Current Word: %s\n", wordCounter, wordArray);
                    insideAWord = 0;            // Reset flag: we are no longer inside a word.
                }
            } else {
                // The character is not whitespace: add it to the word buffer if there's space.
                if (wordIndex < wordArraySize - 1) { // Leave space for null terminator.
                    wordArray[wordIndex] = c;
                    wordIndex++; 
                }
                insideAWord = 1; // Mark that we are inside a word.
            }
        }
    }

    // If read() returned a negative number, an error occurred.
    if (bytesRead < 0) {
        perror("Something is wrong with our code, read returned something less than 0");
    }

    // If the file ended without a trailing whitespace, process the last word.
    if (insideAWord == 1 && wordIndex > 0) {
        wordArray[wordIndex] = '\0'; // Terminate the last word.
        wordCounter++;
        printf("Index %d, Current Word: %s\n", wordCounter, wordArray);
    }

    // Print the total number of words processed.
    printf("Number of words is %d\n", wordCounter);
    if (bytesRead == 0)
        printf("No bytes remaining --> %s is finished reading!\n", filename);

    close(fd); // Close the file.
}



int main(){
    //I will pass in various text files within this directory as args
    




}
