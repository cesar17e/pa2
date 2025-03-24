/*
    Purpose:
    The purpose of this file and subDir is to find out if our Scanning text files function works properly
    Expectation: I e

 */




void scanningFiles(const char *filename){ 
    int fd = open(filename, O_RDONLY); //Retuns a unique int asscoociated with out file ---> -1 if cant be opened
    if (fd == -1) {
        perror("Open failed on scanning files");
        return;
    }  
    FILECOUNTER++; //Increments the files we encounter will need for later usaged for frequencies
    FileHash *fileNode = createFileHash(filename); //Now this contains the file name and local hashtable and a next pointer
    // Now fileNode->ht is the local hash table.

    // Now we insert fileNode into the global list.
    insertFileHash(&fileListHead, fileNode);

    
    char buffer[MAXSIZE+1];
    int bytesRead;
    int wordCounter = 0;
    char wordArray[wordArraySize];
    int wordIndex = 0;
    int insideAWord = 0; // 1 represents if were inside a word and 0 if not

    while ((bytesRead = read(fd, buffer, MAXSIZE)) > 0) { //remeber read retuns an int of bytes it read 0 if no more a <0 if something went wrong
        for (int i = 0; i < bytesRead; i++) { //Goes through our whole buffer
            char c = buffer[i];
            if (isspace(c) || c == '\0' ) { //reached end of word or reached a null terminator cause what if it appears in file
                if(insideAWord == 1){ //Process the word
                    wordArray[wordIndex] = '\0'; //So even if our first wordArray has hello\0 in it then we read i so the wordArray will have I\0ello\0 this will be fine as the string will end at the first \0
                    wordIndex = 0;
                    wordCounter++;
                    printf("Index %d, Current Word: %s\n", wordCounter, wordArray);
                    rewriteWord(wordArray);
                    if(strlen(wordArray)!= 0){
                        printf("Corrected Word, %s\n", wordArray); 
                        //!SEND TO HASH,void insertWord(HashTable *ht, const char *word)

                        //Send to local Hash
                        insertWord(fileNode->ht, wordArray);
                        //Send to global hash
                        insertWord(globalHT, wordArray); 

                    }else{
                        printf("I was full of invalid chars I am empty now, %s\n", wordArray); 
                        //!FOR DEBUGGING PURPOSES 
                    }
                    insideAWord = 0; //If we are in a space we set inside a word to 0
                }
            } else {
                if(wordIndex < wordArraySize-1){ //Ensure space for null terminiator  
                    wordArray[wordIndex] = c;
                    wordIndex++; 
                }
                insideAWord = 1;
            }
        }
    }
