    Cesar Estrada (ce223), Brandon Son (bjs334)
    CS214
    Assignment 2 - Word Frequency

    PROGRAM DESIGN AND OVERVIEW
    ===========================

    The purpose of this program is to scan and analyze given files, which are passed in as two or more arguments. For each input, the program scans the file and does different processes depending on the file type of the scanned file. If the given file is a directory, then the program will recursively traverse through that directory until all files from it are read. If the given file is a text file, then the program will call the scanFile function. If the given file is neither a text file or a directory, then the program will print that the file is neither a directory or a text file and skip the file. 
    For each text file that the program scans, it stores local (specific to that file) frequency in a hashtable as well adding the fileName to a file list, stored as a linked list.
    The program also stores a global hashtable to store the frequency of each word FOR ALL FILES. After the program recursively scans through all the files in the arguments, it have computed both the global frequency of all words and local frequency for each local file. For each file, the program will output the most unusually frequent word, calculated as the word that has the maximum following ratio: (Local Frequency) / (Global Frequency), along with the stipulation that, in the case that two words have the same frequency ratio, the word that comes lexographically first will be selected as the most unusually frequent word. 


    HASH FUNCTION
    =============

    Both the global and local hashtables use the same hash function and share a similar format, more specifically an array with a size of 1000. The hash function works in the following manner: for each character in the word to hash, it multiplies the value of the current hash by 31 and then adds the ASCII value of the current character. Finally, it calculates the modulo 1000 of the result to ensure that it fits somewhere in our hash table. This specific multiplier is chosen because it is a prime number, which in theory ensures minimal collision and optimal performance. Each entry of the hash table is stored as a linked list, with each node containing the word, its frequency, and a pointer to the next node. The pointer to the next node is necessary in case of collision, where two words share the same hash value. This represents a separate chaining strategy to deal with collisions and allows multiple words to be stored in the same hash bucket.

    FILE AND TEXT PROCESSING
    ========================

    As stated before, the program scans all arguments using the scanningFiles method, which behaves differently based on the given file type. If the input is a directory, the searchDirectory method is recursively called to locate text files. If the input is a text file, the scanFile method is called to scan that text file. If the input is neither of these files, it will skip the file and note that it was neither a text file or directory.

    When scanning files for words, special checks are made to ensure that it is a valid word. For our implementation, words are defined as sequences of non-whitespace characters that contain at least one letter (so a word with ONLY numbers are not allowed). Additionally, words must not start with (, [, {, ", or ', and must not end with ), ], }, ", ', ,, ., !, or ?. Also, words are case INSENSITIVE for this project so any uppercase is converted to lowercase (so Hello becomes hello). 
    
    If a valid word is read it is added to a node in a hashtable with the process outlined above. 


    TEST CASES
    ==========

    Test cases are written and accessible from the test directory in P2. There are three subdirectories of test cases: correctnessTesting, designed to ensure accuracy and test for edge cases, overloadTesting, designed to ensure that our program does not break with large inputs, and correctnessTestingOfFunctions, designed to ensure that each individual component of our function works properly.

    correctnessTesting --> testlexographical

    This test case tests the edge case where if two words have the same unusual frequency in a file, the output is the word that comes lexographically first. In this test, text3.txt has the same unusual frequency of apple and banana, so apple is expected. This case works as expected.


    overloadTesting --> 10000words

    As the name implies, this test checks for program durability by having the program scan a folder with two text files, each with 10000 words. This case works as expected.

    overloadTesting --> buffertest
    
    This test is designed to test if our program correctly reads words, even if they are super long. This directory has only one file which has a 501 character word and multiple 500 character words. As expected, the program is able to differentiate these as two different words and works as expected.

    correctnessTestingOfFunctions --> rewriteWordFunction

    This test is designed to assess the accuracy of the rewriteWord function, which is designed to censor out words beginning with (, [, {, ", or ' or ending with ), ], }, ", ', ,, ., !, or ?. It also ensures that upper/lower case does not matter by converting things to lowercase, and ensures that words must have at least one letter. This test works as expected.

    correctnessTestingOfFunctions --> hashFunction

    This test checks to see if the hash function is working as intended. This test hashes the word cat, which has an expected hash of 262, because of the following calculation:
    Initial hash = 0
    hash = (0 * 31) + 99 = 99
    hash = (99 * 31) + 97 = 3166
    hash = (3166 * 31) + 116 = 98262
    Final index = 98262 % 1000 = 262

    This test works as expected.


    correctnessTestingOfFunctions --> scanningFunction

/*
    Purpose:
        This function opens a file given by its filename, reads its contents in chunks,
        and extracts words from the file. A word is defined as a sequence of non-whitespace
        characters. When a word is complete the function prints out the word along with its order of occurence
    Test:
        We will create a text file containing multiple words with spacing, newlines, and punctuation. 
        For example, a file with:
            "Hello World\nThis is   a test."
        Expect the function to print each extracted word in order.
    Expectation:
        - It should correctly detect word boundaries using isspace()
        - It should print each word along with its index.
        - If the file ends without whitespace, the last word should still be processed.
        - It should finally print the total number of words processed.
*/











    
