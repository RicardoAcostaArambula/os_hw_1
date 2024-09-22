
/**
 * Happy and Edge cases:
 * Case 1: No option & no file argument
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
 * Case 4:  filename & num optionals || num optionals & filename
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
/*functions declarations */
int isDigit(char c);
int my_atoi(char s[]);
int isSame(char value[], char target[]);

int main(int argc, char **argv){
    /*No file and no option*/
    int numberOfLines;

    if (argc == 1){
        numberOfLines = 10;
    } else if (argc == 2){
        /*check that second argument is a file name*/
    }
    else if (argc == 3){
        /* Checking for case = ./head -n 10 */
        char n[] = "-n";
        if (isSame(argv[1], n)){
            numberOfLines = my_atoi(argv[2]);
        } else {
            /*rise error for invalid input*/
        }

    } else if (argc == 4){
        /* check for case: 1. filename -n # 2. -n # filename */
        char n[] = "-n";
        /* case when -n goes the second argument */
        if (isSame(argv[1], n)){
            numberOfLines = my_atoi(argv[2]);
        } else if (isSame(argv[2], n)){
        /* case when -n goes the third index argument */
            numberOfLines = my_atoi(argv[3]);
        }
    }
    return 0;
}





/*my_atoi: convert s to integer*/
int my_atoi(char s[]){
    int i, n, sign;
    i=0;
    sign = (s[i]== '-')? -1 : 1;
    if (s[i]=='+' || s[i]=='-')
        i++;

    for (n=0; s[i]!='\0'; i++){
        if (!isDigit(s[i])){
            /*rise error*/

        }
        n = 10 * n + (s[i] - '0');
    }
    return sign * n;
}
/*checks if c is a digit */
int isDigit(char c){
    return '0' <= c  && c <='9';
}

int isSame(char value[], char target[]){
    int i, same;
    i = 0;
    while ((value[i] != '\0' && target[i] != '\0') && (value[i] == target[i]))
        i++;
    same = (value[i] =='\0' && target[i]=='\0');
    return same;
}