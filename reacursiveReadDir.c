



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

