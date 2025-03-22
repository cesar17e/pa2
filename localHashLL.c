#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "localHashLL.h"

// Create a new FileHash node.
FileHash *createFileHash(const char *filename) {
    FileHash *node = malloc(sizeof(FileHash));
    if (!node) {
        perror("Failed to allocate FileHash node");
        return NULL;
    }
    
    // Manually allocate and copy the filename
    size_t len = strlen(filename) + 1; // +1 for the null terminator
    node->filename = malloc(len);
    if (!node->filename) {
        perror("Failed to allocate memory for filename");
        free(node);
        return NULL;
    }
    strcpy(node->filename, filename);
    // Initialize the local hash table for this file.
    node->ht = NULL;
    createHashTable(&(node->ht)); // Updates node->ht directly.
    node->next = NULL;
    return node;
}

// Insert at the beginning of the list.
void insertFileHash(FileHash **head, FileHash *node) {
    if (!node) return;
    node->next = *head;
    *head = node;
}

// Free the list
void freeFileHashList(FileHash *head) {
    while (head) {
        FileHash *temp = head;
        head = head->next;
        free(temp->filename);
        freeHashTable(temp->ht);  // Our freeHashTable() frees a hashtable instance.
        free(temp);
    }
}
