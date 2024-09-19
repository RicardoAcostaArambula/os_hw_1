
/**
 * Happy and Edge cases:
 * Case 1: No option & on file argument
    - When running without any option nor file argument, the head command only 
    copies the first 10 lines of the input it receives on standard input (stdin)
    and disregard any other lines seen on input
 * Case 2: file argument
    - When running with a filename argument, the head command do not read from standard input 
    but from the file designated by that file argument. 
        - if they cannot open the file in read-mode, they display an appropriate error message 
        on standard error and exit
        - it only handles one file argument
 * Case 3: -n [num] option
    - when run with the -n [num] option, the head command behave as specified above but replace
    the number of 10 lines by the approtiate ammount specified by [num]
        - if num is zero, they do nothing
        - if the ammount is negative, head do nothing either 
        - you are supposed to perform the string-to-integer conversion with code written on your
        own; not usign atoi or strtol which will be level-3 operations 
filename & num optionals
    - they can be combine in any resonable order
* Notes
    1. filename & num optionals
        1.1 they can be combine in any resonable order
    2. Errors
        2.1 if no error condition arises, head return the success condition code (zero)
        2.2 if an error occurs, they return a non-zero failure codition code (one)
            2.2.1 but print the errno as fprintf(stderr, "Error with read(): %s\n", strerror(errno));
    3. Memory   
        3.1 any memory allocated on the heap with malloc or calloc must be returned using free. 
        3.2 All accesses to arrays must use correct indices; you may want to double-check on this. 
        3.3 All files opened with open must be closed with close. 
*/

#include <stdio.h>

int main(int argc, char **argv){
    return 0;
}