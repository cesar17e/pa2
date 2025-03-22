#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include "hashTable.h"

/*
This contains the hash function, We dont want negatives in our code so make it return an unsigned integer index
*/

void createHashTable(HashTable **ht) {
    *ht = malloc(sizeof(HashTable));
    if (!*ht) {
        perror("Failed to allocate hash table");
        return;
    }
    for (int i = 0; i < TABLE_SIZE; i++) {
        (*ht)->buckets[i] = NULL;
    }
    (*ht)->totalWords = 0;
}


unsigned int hash(const char *word) { 
    unsigned int hash = 0;
    while (*word) {
        hash = (hash * 31) + *word;
        word++;
    }
    return hash % TABLE_SIZE;
}

void insertWord(HashTable *ht, const char *word) {
    unsigned int index = hash(word); 
    wordFreq *head = ht->buckets[index]; //Our head
    
    // Search for the word in the bucket
    while (head) {
        if (strcmp(head->word, word) == 0) {
            head->count++;
            ht->totalWords++;
            return;
        }
        head = head->next;
    }
    
    // Word not found so we create a new entry, begin chaining process
    wordFreq *newEntry = malloc(sizeof(wordFreq));
    if (!newEntry) {
        perror("allocation failed!");
        return;
    }
    newEntry->word = malloc(strlen(word) + 1); //The +1 for the null terminator
    if (!newEntry->word) {
        perror("Word allocation failed!");
        free(newEntry);
        return;
    }
    strcpy(newEntry->word, word);  // Copy the content of word into newEntry->word
    newEntry->count = 1; //Set the count of the word to 1
    newEntry->next = ht->buckets[index]; //Adds to the front of the list
    ht->buckets[index] = newEntry; //Sets the head to be the newEntry saves it
    ht->totalWords++;
}

//Free function since we use malloc alot

void freeHashTable(HashTable *ht) {
    for (int i = 0; i < TABLE_SIZE; i++) {
        wordFreq *entry = ht->buckets[i];
        while (entry) {
            wordFreq *temp = entry;
            entry = entry->next;
            free(temp->word);
            free(temp);
        }
        ht->buckets[i] = NULL;
    }
    free(ht);  // Finally free the hash table structure itself.

}
