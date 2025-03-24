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
#include "hashTable.h" //Includes our hashtable function
#include "localHashLL.h" //Includes the linked list that stores our local hashtables
#include "wordProcessing.h" //Includes the word processing of functions


#define MAXSIZE 8192 //This is for buffer read and path file names
#define wordArraySize 500 // We can make this bigger 


//The global hash table for overall frequency.
HashTable *globalHT = NULL;
//Linked list head pointer to store local hash tables for each file.
FileHash *fileListHead = NULL;
int FILECOUNTER = 0;
    

/*
    Will check if the file name ends witha .txt to be scanned
    Will return 0 if false and 1 if true
    We will use strncmp with pointer arithmatci to reach the last 4 chars and not use strcmp because of null terminator might not be added
*/

int txtFile(const char *name){
    int len = strlen(name); //strlen does not include the null terimator
    //We need to chech for the last 4 chars .txt so if the name is less than 4 chars we return 0
    if(len < 4) {
        return 0;
    }
    //Now we use strcmp to compare the last 4 chars of our string to .txt
    //So name pointer arithmatic to end of last char (not \0) then -4 to get to the begging of the .
    if (strncasecmp(name + (len-4), ".txt", 4) == 0){ // == 0 means its the same anything else is not 
        return 1;
    }else{
        return 0;
    }
}

/*
!Work in progress of scanning the files and generating the words, couting the words works fine everything seems fine
!Now onto generating the words using these guidelines
!- Words A word is a sequence of non-whitespace characters containing at least one letter; --> so ((!!)) would not be sent as its just the \0 not letter \0 is a whitespace
!- Not starting with (, [, {, ", or ’; and not ending with ), ], }, ", ’, ,, ., !, or ?.
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

    if (bytesRead < 0) { //If read gets negative input somethings wrong
        perror("Something is wrong with our code, read returned something less than 0");
    }

    if (insideAWord == 1 && wordIndex > 0) { //If the file does not end with a whitespace we must process the last word
        wordArray[wordIndex] = '\0'; //So even if our first wordArray has hello\0 in it then we read i so the wordArray will have I\0ello\0 this will be fine as the string will end at the first \0
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
    }

    printf("Number of words is %d", wordCounter);
    if(bytesRead ==0 ) printf("\nNo bytes remaining--> %s is finsihed reading!\n", filename);

    close(fd); //close the file
}


/*
Here we will recursivley search through a directory
Somethings we need to know before starting this
    -On the first go we will pass the arg string and then open it with openDIR
    -We will then traverse the contents with the struct *dirent
    -To get the type of the Directory Entry we use the struct stat, which needs a path to open it
    -Now since this is recursion we need to construct a path for the struct stat to work as it needs a path to know where its looking at
        -This will also be needed when we scan the file using open because it needs the path to get there
        -So to construct the path we will keep recursively call the dirname each time while we add onto it using strcpy and strcat
    -Now that we have the correct path we can use it in stat and then when scanning files into open
    -Remember these rules stated by the assignment
        1. Names beginning with a period (.) are ignored
        2. Directories are recursively traversed
        3. Regular files with names ending in “.txt” are scanned
        4. Any other files are ignored
*/

void searchDirectory(const char *dirName){ 
    DIR* dir = opendir(dirName);
    if(dir == NULL){
        perror("opendir failed");
        return;
    }
    //We will now travese the directory recursively 
    struct dirent* entry; 
    struct stat pathStatus; 

    while((entry = readdir(dir)) != NULL){
        if(entry->d_name[0] == '.'){  //Ignoring names with (.) at index 0
            continue;
        }
        //We now need to construct the correct path to the file we are at --> Example 
        char path[MAXSIZE] = {0};

        int directoryLength = strlen(dirName);
        int entryNameLength = strlen(entry -> d_name);
        //!this doesn't necessarily fix the issue, but it just adds checks to ensure that program returns if length is too long
        if(directoryLength + 1 + entryNameLength >= MAXSIZE){
            printf("Error!!! Path too long: %s/%s\n", dirName, entry ->d_name);
            return;
        }
        strcpy(path, dirName); //--> On first go foo
        strcat(path, "/"); //--> foo/
        strcat(path, entry->d_name); // foo/bar is now our the path to the entry we are looking at 
        //!Important for above
        //!This might lead to overflow so need to find a better way to make it better, maybe malloc and realloc?
        //!Look into later in can be fixed quick

        //Now that we have the correct path we can now get the type of the entry
        int type = stat(path, &pathStatus);
        if (type != 0) { // Something went wrong, stat returns 0 on success
            perror("Something wrong with stat on searchDir");
            continue;
        }

        if (S_ISREG((pathStatus.st_mode))) { //If its a regular file
            int checkIfTxt = txtFile(path);
            if(checkIfTxt == 0){
                printf("Skipping: Not a txt file just a regular one %s\n", path);
            }else{
                printf("This is a text file %s\n", path);
                scanningFiles(path);
            }
        } else if ((S_ISDIR(pathStatus.st_mode))) { // If its a directory we recursively call our function again to enter again with the name of path
            printf("directory: %s\n", entry->d_name);
            searchDirectory(path);
        } else {
            printf("Skipping %s: It is not a file or a directory\n", entry->d_name);
        }
    }
    closedir(dir);  //close the directory
}


void printFrequencies(HashTable *ht) {
    if (!ht || ht->totalWords == 0) {
        printf("No words to compute frequency.\n");
        return;
    }
    
    printf("Word Frequencies:\n");
    for (int i = 0; i < TABLE_SIZE; i++) {
        wordFreq *entry = ht->buckets[i];
        while (entry != NULL) {
            double freq = (double) entry->count / ht->totalWords;
            printf("Word: %-15s Count: %-5d Frequency: %.4f\n",
                   entry->word, entry->count, freq);
            entry = entry->next;
        }
    }
}


double getGlobalFrequency(HashTable *ht, char *word){
    if(!ht || ht ->totalWords ==0){
        printf("Error! No words in Hashtable \n");
        return 0.0;
    }
    unsigned int index = hash(word);
    wordFreq *entry = ht -> buckets[index];
    while(entry != NULL){
        if(strcmp(entry ->word, word) == 0){
           // printf("getGlobalFrequency Method Found ! \n ");
            return (double) entry-> count / ht -> totalWords;
        }
        entry = entry->next;
    }
    printf("getGlobalFrequency Method did Not Find Word !");
}

void findMostUnusualWord(FileHash *fileNode, HashTable *globalHT){
    if(!fileNode || !fileNode -> ht || !globalHT){
        printf("Error! One of the arguments were null or not initalized properly");
        return;
    }
    HashTable *localHT = fileNode -> ht;
    char *weirdWord = NULL;
    double maxRatio = 0.0;

    for(int i = 0; i < TABLE_SIZE; i++){
        wordFreq *entry = localHT->buckets[i];
        while(entry != NULL){
            double localFreq = (double) entry -> count / localHT-> totalWords;
            double globalFreq = getGlobalFrequency(globalHT, entry->word);
            double ratio = localFreq / globalFreq;


            if(ratio > maxRatio){
                maxRatio = ratio;
                weirdWord = entry->word;
            }
            //!IF THE TWO WORDS HAVE THE SAME FREQ, NEED TO COMPARE LEXIOGRAPHICALLY
            else if(ratio == maxRatio){
                if(strcmp(entry -> word, weirdWord) < 0){
                    weirdWord = entry ->word;
                }
            }

            entry = entry->next;
        }
    }

    if(weirdWord){
        printf("File: %s, Most Unusual Word: %s, Ratio: %.4f\n", fileNode -> filename, weirdWord, maxRatio);
    } else {
        printf("File: %s, NO Unusual Words found.\n", fileNode -> filename);
    }
}

void printWeirdWords(FileHash *fileListHead, HashTable *globalHT){
    FileHash *current = fileListHead;
    while(current != NULL){
        findMostUnusualWord(current, globalHT);
        current = current->next;
    }
}

/*
Our program will take in aruguements, we will use stat to determine whether they are of dir or regular files
Once we figure out the type 
    - If a directory we will traverse it recusivley and then send each txt file to be scanned
    - If it is a regular file ending in txt we will send it out to be scanned
*/
int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Not Enough Arguements");
        return 1;
    }

    //Create the global hashtable
    createHashTable(&globalHT); // Now globalHT points to a initialized hash table.
    
    //Process the inputs
    for(int i =0; i<argc; i++){
        struct stat pathStatus; //This will tell us the type of file we are reading
        int type = stat(argv[i],&pathStatus);
        
        if(type!=0){  //If it doesnt return 0 then something went wrong, print out an error of this arg and then continue to the next arg
            perror(argv[i]);
            continue;
        }
        //So now the struct is assigned to the file/dir and using .st_mode it will give us the type if its a file or a directory
        if (S_ISREG((pathStatus.st_mode))) { // If its a regular file
            int checkIfTxt = txtFile(argv[i]);
            //Checking if my txt file identifier works
            if(checkIfTxt == 0){
                printf("Not a txt file just a reg one %s\n", argv[i]);
            }else{
                printf("This is a text file %s\n", argv[i]);
                scanningFiles(argv[i]);
            }
        }else if (S_ISDIR(pathStatus.st_mode)) { // If its a directory we need to scan the contents inside recursivley
            searchDirectory(argv[i]);
        } else {
            printf("Skipping %s: It is not a file or a directory\n", argv[i]);
        }
    }


    //!NOW ALL IS PROCESSED PROCEED
    printFrequencies(globalHT);

    printWeirdWords(fileListHead, globalHT);

    //!CALL TO GLOBAL 
    //free both at the end
    freeHashTable(globalHT); // Free the global hash table.
    freeFileHashList(fileListHead); // Free the linked list of local hash tables, it includes freeHashTable within the funcion freeing the many local hashTables

}

