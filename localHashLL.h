#ifndef FILE_HASH_H
#define FILE_HASH_H
#include "hashTable.h"  // This header defines the hashtable

typedef struct FileHash {
    char *filename;        // Name or path of the file.
    HashTable *ht;         // Local hash table for this file.
    struct FileHash *next; // Next file in the list.
} FileHash;

// Creates a new filehash node for a given filename.
FileHash *createFileHash(const char *filename);

// Inserts node at the beginning of the list.
void insertFileHash(FileHash **head, FileHash *node);

// Frees the entire localHashLL list, including each files hash table
void freeFileHashList(FileHash *head);

#endif //The guards


/*
This header contains the .c files for the linked list structure which will hold the copies of the local file hashTables we will make to hold the data of the local hashtables made
*/