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
#define MAXSIZE 8192 //This is for buffer read and path file names

/*
Purpose: The purpose of this file and subdir is to see if my searchDirectory functions properly
Test: I will provide this file with many sub-directorys to see if it correctly traveres the entire directory Properly
Expectation: I will provide print statements for each pathName made and this will tell me if it is correctly traversing the directory
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
            printf("This is a regular file one %s\n", path);


        } else if ((S_ISDIR(pathStatus.st_mode))) { // If its a directory we recursively call our function again to enter again with the name of path
            printf("directory: %s\n", entry->d_name);
            searchDirectory(path);
        } else {
            printf("Skipping %s: It is not a file or a directory\n", entry->d_name);
        }
    }
    closedir(dir);  //close the directory
}


int main(int argc, char *argv[]){
    if (argc < 2) {
        printf("Not Enough Arguements");
        return 1;
    }


    for(int i =0; i<argc; i++){
        struct stat pathStatus; //This will tell us the type of file we are reading
        int type = stat(argv[i],&pathStatus);
        
        if(type!=0){  //If it doesnt return 0 then something went wrong, print out an error of this arg and then continue to the next arg
            perror(argv[i]);
            continue;
        }
        //So now the struct is assigned to the file/dir and using .st_mode it will give us the type if its a file or a directory
        if (S_ISREG((pathStatus.st_mode))) { // If its a regular file
            printf("This is a regular file %s\n", argv[i]);
        
        }else if (S_ISDIR(pathStatus.st_mode)) { // If its a directory we need to scan the contents inside recursivley
            printf("We will begin recursivley searching %s", argv[i]);
            searchDirectory(argv[i]);
        } else {
            printf("Skipping %s: It is not a file or a directory\n", argv[i]);
        }
    }

}

