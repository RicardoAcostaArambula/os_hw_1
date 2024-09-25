
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
#include <errno.h>
#include <stdlib.h>
/*do we need to include all of them?*/
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>


#define BUFFER_SIZE 4096
/*functions declarations */
int isDigit(char c);
int my_atoi(char s[]);
int my_strlen(char c[]);
int isSame(char value[], char target[]);
ssize_t my_write(int fd, const char *buf, size_t count);
ssize_t read_until_newline(int fd, char *buf, size_t max_size);

int main(int argc, char **argv){
    int numberOfLines, fd, rc;
    char *current_line;
    char **all_lines;
    char byte;
    char buf[BUFFER_SIZE];
    ssize_t read_res;
    size_t bytes_read_in, i;
    size_t current_line_size, current_line_alloc, new_size;
    size_t all_lines_size, all_lines_alloc;
    void *ptr;
    size_t *all_lines_lengths;
    current_line = NULL;
    current_line_size = (size_t) 0;
    current_line_alloc = (size_t) 0;
    all_lines = NULL;
    all_lines_lengths = NULL;
    all_lines_size = (size_t) 0;
    all_lines_alloc = (size_t) 0;
    numberOfLines = 10;
    rc = 0;
    if (argc == 1){
        /*No file and no option*/
        while (numberOfLines-- > 0){
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
        }

    } else if (argc == 2){
        /*CASE: ./head filename */
        /*check that second argument is a file name*/
        char *filename = argv[1];
        fd = open(filename, O_RDONLY);
        if ( fd < ((ssize_t) 0)){
            /*Error: something happend openning the file*/
            fprintf(stderr, "Error: could not open file %s\n", strerror(errno));
            rc = 1;
            goto cleanup_and_return;
        }
        /**
         * we are reading while there is still content in the file
         * now we need to process what was store in the buffer
         * 
        */
        /******************************************************************************************/
        /******************************************************************************************/
        /******************************************************************************************/
        /******************************************************************************************/
        /******************************************************************************************/
        while (1){
            read_res = read(fd, buf, sizeof(buf));
            if (read_res < ((size_t) 0)){
                fprintf(stderr, "error with read(): %s\n", strerror(errno));
                rc = 1;
                goto cleanup_and_return;
            }
            if (read_res == ((size_t) 0)){
                break;
            }
            if ( all_lines_size == 10){
                break;
            }
            bytes_read_in = (size_t) read_res;
            for(i = (size_t) 0; i < bytes_read_in; i++){
                byte = buf[i];


                if (current_line == NULL){
                    current_line_alloc = (size_t) 1;
                    ptr = calloc(current_line_alloc, sizeof(char));
                    if (ptr == NULL){
                        fprintf(stderr, "Error: could not allocate memory\n");
                        rc = 1;
                        goto cleanup_and_return;
                    }
                    current_line = (char *) ptr;
                } else {

                    if (current_line_size >= current_line_alloc){

                        new_size = current_line_alloc + current_line_alloc;
                        if (new_size < current_line_alloc){
                            fprintf(stderr, "Error: cannot handle memory that large\n");
                            rc = 1;
                            goto cleanup_and_return;
                        }
                        ptr = reallocarray(current_line, new_size, sizeof(char));
                        if (ptr == NULL){
                            fprintf(stderr, "Error: could not allocate memory\n");
                        }
                        current_line_alloc = new_size;
                        current_line = ptr;
                    }
                }
                current_line[current_line_size] = byte;
                current_line_size++;

                if (byte == '\n'){
                    if (all_lines == NULL){
                        all_lines_alloc = (size_t) 1;
                        ptr = calloc(all_lines_alloc, sizeof(char *));
                        if (ptr == NULL){
                            fprintf(stderr, "Error: count not allocate memory\n");
                            rc = 1;
                            goto cleanup_and_return;
                        }
                        all_lines = (char **) ptr;
                        ptr = calloc(all_lines_alloc, sizeof(size_t));
                        if (ptr == NULL){
                            fprintf(stderr, "Error: count not allocate memory\n");
                            rc = 1;
                            goto cleanup_and_return;
                        }
                        all_lines_lengths = (size_t *)  ptr;
                    } else {
                        if (all_lines_size >= all_lines_alloc) {
                            new_size = all_lines_alloc + all_lines_alloc;
                            if (new_size < all_lines_alloc) {
                                fprintf(stderr, "Error: cannot handle memory that large\n");
                                rc = 1;
                                goto cleanup_and_return;
                            }
                            ptr = reallocarray(all_lines, new_size, sizeof(char *));
                            if (ptr == NULL) {
                                fprintf(stderr, "Error: could not allocate memory\n");
                                return 1;
                            }
                            all_lines = (char **) ptr;
                            ptr = reallocarray(all_lines_lengths, new_size, sizeof(size_t));
                            if (ptr == NULL) {
                                fprintf(stderr, "Error: could not allocate memory\n");
                                rc = 1;
                                goto cleanup_and_return;
                            }
                            all_lines_lengths = (size_t *) ptr;
                            all_lines_alloc = new_size;
                        }
                    }
                    all_lines[all_lines_size] = current_line;
                    all_lines_lengths[all_lines_size] = current_line_size;
                    all_lines_size++;
                    current_line = NULL;
                    current_line_size = (size_t) 0;
                    current_line_alloc = (size_t) 0;
                }
            }

        }
        /**outside while infinite loop
         * hanlding the case when line did not end on newline character 
         * but rather on EOF character
        */

        if (current_line != NULL) {
            if (all_lines == NULL) {
                all_lines_alloc = (size_t) 1;
                ptr = calloc(all_lines_alloc, sizeof(char *));
                if (ptr == NULL) {
                    fprintf(stderr, "Error: could not allocate memory\n");
                    rc = 1;
                    goto cleanup_and_return;
                }
                all_lines = (char **) ptr;
                ptr = calloc(all_lines_alloc, sizeof(size_t));
                if (ptr == NULL) {
                    fprintf(stderr, "Error: could not allocate memory\n");
                    rc = 1;
                    goto cleanup_and_return;
                }
                all_lines_lengths = (size_t *) ptr;
            } else {
                if (all_lines_size >= all_lines_alloc) {
                    new_size = all_lines_alloc + all_lines_alloc;
                    if (new_size < all_lines_alloc) {
                        fprintf(stderr, "Error: cannot handle memory that large\n");
                        rc = 1;
                        goto cleanup_and_return;
                    }
                    ptr = reallocarray(all_lines, new_size, sizeof(char *));
                    if (ptr == NULL) {
                        fprintf(stderr, "Error: could not allocate memory\n");
                        rc = 1;
                        goto cleanup_and_return;
                    }
                    all_lines = (char **) ptr;
                    ptr = reallocarray(all_lines_lengths, new_size, sizeof(size_t));
                    if (ptr == NULL) {
                        fprintf(stderr, "Error: could not allocate memory\n");
                        rc = 1;
                        goto cleanup_and_return;
                    }
                    all_lines_lengths = (size_t *) ptr;
                    all_lines_alloc = new_size;
                }
            }
            all_lines[all_lines_size] = current_line;
            all_lines_lengths[all_lines_size] = current_line_size;
            all_lines_size++;
            current_line = NULL;
            current_line_size = (size_t) 0;
            current_line_alloc = (size_t) 0;
        }

        /******************************************************************************************/
        /******************************************************************************************/
        /******************************************************************************************/
        /******************************************************************************************/
        /******************************************************************************************/

        /*
         * You are suppose to have the lines in the array of pointers
         * now you need to print them. 
         **/

        for(i= (size_t) 0; i < 10; i++){
            if (my_write(1, all_lines[i], all_lines_lengths[i]) < ((size_t)0)){
                fprintf(stderr, "Error with my_write(): %s\n", strerror(errno));
                rc = 1;
                goto cleanup_and_return;
            }
        }




    } else if (argc == 3){
        /* Checking for case = ./head -n 10 */
        char n[] = "-n";
        if (isSame(argv[1], n)){
            numberOfLines = my_atoi(argv[2]);

            while (numberOfLines-- > 0){
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
            }

        } else {
            /*rise error for invalid input*/
        }

    } else if (argc == 4){
        /* check for case: 1. ./head filename -n # */
        char n[] = "-n";
        /* case when -n goes the second argument */
        if (isSame(argv[2], n)){
            numberOfLines = my_atoi(argv[3]);
            /* Instructions read from file  the number of lines previously defined*/





        } else if (isSame(argv[2], n)){
            /* case when ./head -n # filename  */  


            numberOfLines = my_atoi(argv[3]);
        }
    }


cleanup_and_return:
    if (current_line !=NULL){
        free(current_line);
    }
    if (all_lines != NULL){
        for (i=(size_t)0; i < all_lines_size; i++){
            free(all_lines[i]);
        }
        free(all_lines);
        free(all_lines_lengths);
    }
    return rc;
}


/*writes to the screen all that is in buf*/
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

/**
 * Reads until it finds a newline character
 * fd is where it is reading from
 * buf is where it is writing what was read
 * max is the size of that is being read
 * @returns the total that was read
*/
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
        buf[total_read++] = ch;
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
int my_strlen(char c[]){
    int i;
    for(i=0;c[i]!='\0';i++)
        ;
    return i;
}
