#ifndef HASH_TABLE_H
#define HASH_TABLE_H

typedef struct wordFreq { //We inlcude it in our headerFile so that it is no longer hidden
    char *word;
    int count;
    struct wordFreq *next;
} wordFreq;

#define TABLE_SIZE 1000

// Struct for HashTable structure that holds an array of buckets and a total counter.
typedef struct {
    wordFreq *buckets[TABLE_SIZE];
    unsigned int totalWords;
} HashTable;

// Function prototypes:
unsigned int hash(const char *word);
void insertWord(HashTable *ht, const char *word);
void freeHashTable(HashTable *ht);

#endif //For the gaurd stuff
