#include <stdio.h>
#include <stdlib.h> 
#include <string.h>  
//Down below are the stuff we need
#include <sys/stat.h> 
//Tells us the type of each argument
//On success, stat() returns 0.
#include <fcntl.h> //Adds the modes
#include <dirent.h> //Holds open dir etc
#include <unistd.h> //Holds open files etc
#include <ctype.h> 

#define HASH_SIZE 1000;

// The hash function to test
unsigned int hash(const char *word) {
    unsigned int hash = 0;
    while (*word) {
        hash = (hash * 31) + *word;
        word++;
    }
    return hash % HASH_SIZE;
}

void test_hash_cat() {
    const char *word = "cat";
    unsigned int expected_index = 262; 
    unsigned int actual_index = hash(word);

    printf("Testing hash(\"%s\")...\n", word);
    printf("-> Computed index: %u\n", actual_index);
    printf("-> Expected index: %u\n", expected_index);

}

int main() {
    test_hash_cat();
    return 0;
}