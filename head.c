
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
#include <unistd.h>
#include <string.h>
/*functions declarations */
int isDigit(char c);
int my_atoi(char s[]);
int isSame(char value[], char target[]);
ssize_t my_write(int fd, const char *buf, size_t count);
ssize_t read_until_newline(int fd, char *buf, size_t max_size);

int main(int argc, char **argv){
    int numberOfLines, fd;
    char buf[4096];
    ssize_t read_res;
    size_t bytes_read_in;
    /*No file and no option*/
    if (argc == 1){
        numberOfLines = 10;

        while (numberOfLines > 0){
            /*read from input and at each '\n' new line character prints it to the screen*/
            read_res = read_until_newline(0, buf, sizeof(buf));

            if (read_res < ((ssize_t) 0)){
                /*
                - print error probably using strerror 
                - exit the program
                */
            }
            if (read_res == ((ssize_t) 0)){
                /*we have hit the EOF condition*/
                break;
            }
            bytes_read_in = read_res;
            /*call write*/
            my_write(1, buf, bytes_read_in);
            numberOfLines--;
        }

    } else if (argc == 2){
        /*check that second argument is a file name*/
    } else if (argc == 3){
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



ssize_t my_write(int fd, const char *buf, size_t count){
    size_t total_written = 0;
    ssize_t bytes_written;
    if (count == ((size_t) 0)) return (ssize_t) 0;
    while (total_written < count){
        bytes_written = write(1, buf + total_written, count - total_written);
        if (bytes_written < ((size_t) 0)) {
            /*Error*/
            return bytes_written;
        }
        if (bytes_written == ((size_t) 0)){
            /*Nothing to be written*/
            return total_written;
        }
        total_written+=bytes_written;
    }
    return total_written;
} 
ssize_t read_until_newline(int fd, char *buf, size_t max_size){
    ssize_t total_read = 0;
    char ch;
    while (total_read < max_size){
        ssize_t bytes_read = read(fd, &ch, 1);
        if (bytes_read < ((size_t) 0)){
            /*Error*/
            return bytes_read;
        } else if (bytes_read == ((size_t)0)) {
            /*EOF reach*/
            break;
        }
        if( ch=='\n'){
            break;
        }
    }
    buf[total_read] = '\0';
    return total_read;

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