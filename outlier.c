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
#define MAXSIZE 4096 //This is for buffer read and path file names
#define wordArraySize 500 // We can make this bigger 
// local
// global

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
    Here we will check our new string and organize to lower case and rid of characters that arent allowed as defined by project
        !- Words A word is a sequence of non-whitespace characters containing at least one letter; not
        !-  Starting with (, [, {, ", or ’; and not ending with ), ], }, ", ’, ,, ., !, or ?.
        !-In rewrite word we will use memove and pointer arithmatic so we skip all the non valid chars 
        !KEY WORD- MUST CONTAIN ONE LETTER  
*/
int lowercaseAndcheckforletter(char *word, int size){
    int hasALetter = 0; //
    for(int i =0; i < size; i++){
        word[i] = tolower(word[i]);
        if (isalpha(word[i])) {
            hasALetter = 1;
        }
    }
    return hasALetter; //If it returns 1 it has a letter, 0 if not
}


int frontCharCheck(char c){ //Returns 1 if true 0 if false
    if (c == '(' || c == '[' || c == '{' || c == '\"' || c == '\''){  
        return 1;
    }else{
        return 0;
    }
}
int lastCharCheck(char c){ //Returns 1 if true 0 if false
    if (c == ')' || c == ']' || c == '}' || c == '\"' || c == '\'' || c == ',' || c == '.' || c == '!' || c == '?'){
        return 1;
    }else{
        return 0;
    }
}

void rewriteWord(char *word){
    if(word == NULL){  //
        perror("String is empty find out why, in rewriteword");
        return; 
    } 

    int len = strlen(word); //Stops at the first null
    //Find the new start to use with memcopy
    int start = 0;

    while (start < len && (frontCharCheck(word[start]) == 1)) {
        start++;
    }

    //Next we need the ending valid char
    int end = len - 1; //Gives us ending index
    while (end >= start && (lastCharCheck(word[end]) ==1)) {
        end--;
    }

    //Now we have to calculate the new length
    int newLen = end - start + 1; 
    if (newLen <= 0) { //An exmaple of this happening ((!!)) will become nothing so newline is 0
        // The word became empty or invalid
        word[0] = '\0';
        return;
    }

    //Now we have to copy the substring
    if (start > 0) {
        memmove(word, word + start, newLen); //Copying 
    }
    word[newLen] = '\0'; //Now word is ready to be moved onto the hash-table

    //Before leaving lets make it all lowercase so that HELLO == hello and check if it contains at least one letter!
    int checkForLetter = lowercaseAndcheckforletter(word, newLen); 
    if(checkForLetter == 0){
        word[0] = '\0';
        return;
    }
    return;
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
                        //!SEND TO HASH
                        //Send to local hash
                        //Send to global hash
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
            //!SEND TO HASH
        }else{
            printf("I was full of invalid chars I am empty now, %s\n", wordArray); 
            //!FOR DEBUGGING PURPOSES 
        }
    }

    printf("Number of words is %d", wordCounter);
    if(bytesRead ==0 ) printf("\nNo bytes remaining--> %s is finsihed reading!\n", filename);

    close(fd); //close the file

    //SCAN THE LOCAL HASH
    //F
    //rest local
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


    //!CALL TO GLOBAL 
    //free both at the end

}