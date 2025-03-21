#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include "hashTable.h"

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
    wordFreq *entry = ht->buckets[index];
    
    // Search for the word in the bucket
    while (entry) {
        if (strcmp(entry->word, word) == 0) {
            entry->count++;
            ht->totalWords++;
            return;
        }
        entry = entry->next;
    }
    
    // Word not found; create a new entry
    wordFreq *newEntry = malloc(sizeof(wordFreq));
    if (!newEntry) {
        perror("allocation failed!");
        return;
    }
    newEntry->word = strdup(word);
    if (!newEntry->word) {
        perror("strdup failed!");
        free(newEntry);
        return;
    }
    newEntry->count = 1;
    newEntry->next = ht->buckets[index];
    ht->buckets[index] = newEntry;
    ht->totalWords++;
}

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
}
