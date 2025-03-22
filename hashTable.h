#ifndef HASH_TABLE_H
#define HASH_TABLE_H

#define TABLE_SIZE 1000 //The size for our hashtable

//This is the struct content in which we will add to the indeces of our hashtable it contains the information we will need
typedef struct wordFreq {
    char *word;
    int count;
    struct wordFreq *next;
} wordFreq;

//Struct for HashTable structure that holds an array of buckets and a totalWords counter
typedef struct {
    wordFreq *buckets[TABLE_SIZE]; //An array of wordFreqs
    unsigned int totalWords; //Will tell us the total words in the specified file
} HashTable;

//The function prototypes we will use
void createHashTable(HashTable **head); 
unsigned int hash(const char *word);
void insertWord(HashTable *ht, const char *word);
void freeHashTable(HashTable *ht);

#endif //For the gaurd stuff
