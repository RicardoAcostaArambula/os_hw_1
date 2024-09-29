#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
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
    int numberOfLines, fd, rc, length;
    char *current_line;
    char *message;
    char **all_lines;
    char byte;
    char buf[BUFFER_SIZE];
    ssize_t reading_result;
    size_t bytes_read_in, i;
    size_t current_line_size, current_line_alloc, new_size;
    size_t size_of_all_lines, all_lines_alloc;
    void *ptr;
    size_t *all_lines_lengths;

    current_line = NULL;
    current_line_size = (size_t) 0;
    current_line_alloc = (size_t) 0;
    all_lines = NULL;
    all_lines_lengths = NULL;
    size_of_all_lines = (size_t) 0;
    all_lines_alloc = (size_t) 0;
    numberOfLines = 10;
    rc = 0;
    if (argc == 1){
        /*No file and no option*/
        while (numberOfLines-- > 0){
            /*read from input and at each '\n' new line character prints it to the screen*/
            reading_result = read_until_newline(0, buf, sizeof(buf));
            if (reading_result < ((ssize_t) 0)){
                message = "Error: an error ocurr with read()\n";
                length = my_strlen(message);
                my_write(2, message, length);
                close(fd);
                rc = 1;
                goto cleanup_and_return;
            }
            if (reading_result == ((ssize_t) 0)){
                /*we have hit the EOF condition*/
                break;
            }
            bytes_read_in = reading_result;
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
            message = "Error: could not open file %s\n";
            length = my_strlen(message);
            my_write(2, message, length);
            close(fd);
            rc = 1;
            goto cleanup_and_return;
        }
        /**
         * we are reading while there is still content in the file
         * now we need to process what was store in the buffer
         * 
        */
        while (1){
            reading_result = read(fd, buf, sizeof(buf));
            if (reading_result < ((size_t) 0)){
                message = "Error: an error ocurr with read()\n";
                length = my_strlen(message);
                my_write(2, message, length);
                close(fd);
                rc = 1;
                goto cleanup_and_return;
            }
            if (reading_result == ((size_t) 0)){
                break;
            }
            if ( size_of_all_lines == 10){
                break;
            }
            bytes_read_in = (size_t) reading_result;
            for(i = (size_t) 0; i < bytes_read_in; i++){
                byte = buf[i];


                if (current_line == NULL){
                    current_line_alloc = (size_t) 1;
                    ptr = calloc(current_line_alloc, sizeof(char));
                    if (ptr == NULL){
                        message = "Error: could not allocate memory\n";
                        length = my_strlen(message);
                        my_write(2, message, length);
                        close(fd);
                        rc = 1;
                        goto cleanup_and_return;
                    }
                    current_line = (char *) ptr;
                } else {

                    if (current_line_size >= current_line_alloc){

                        new_size = current_line_alloc + current_line_alloc;
                        if (new_size < current_line_alloc){
                            message = "Error: cannot handle memory that large\n";
                            length = my_strlen(message);
                            my_write(2, message, length);
                            close(fd);
                            rc = 1;
                            goto cleanup_and_return;
                        }
                        ptr = reallocarray(current_line, new_size, sizeof(char));
                        if (ptr == NULL){
                            message = "Error: could not allocate memory\n";
                            length = my_strlen(message);
                            my_write(2, message, length);
                            close(fd);
                            rc = 1;
                            goto cleanup_and_return;
                            
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
                            message = "Error: could not allocate memory\n";
                            length = my_strlen(message);
                            my_write(2, message, length);
                            close(fd);
                            rc = 1;
                            goto cleanup_and_return;
                        }
                        all_lines = (char **) ptr;
                        ptr = calloc(all_lines_alloc, sizeof(size_t));
                        if (ptr == NULL){
                            message = "Error: could not allocate memory\n";
                            length = my_strlen(message);
                            my_write(2, message, length);
                            close(fd);
                            rc = 1;
                            goto cleanup_and_return;
                        }
                        all_lines_lengths = (size_t *)  ptr;
                    } else {
                        if (size_of_all_lines >= all_lines_alloc) {
                            new_size = all_lines_alloc + all_lines_alloc;
                            if (new_size < all_lines_alloc) {
                                message = "Error: cannot handle memory that large\n";
                                length = my_strlen(message);
                                my_write(2, message, length);
                                close(fd);
                                rc = 1;
                                goto cleanup_and_return;
                            }
                            ptr = reallocarray(all_lines, new_size, sizeof(char *));
                            if (ptr == NULL) {
                                message = "Error: could not allocate memory\n";
                                length = my_strlen(message);
                                my_write(2, message, length);
                                close(fd);
                                rc = 1;
                                goto cleanup_and_return;
                            }
                            all_lines = (char **) ptr;
                            ptr = reallocarray(all_lines_lengths, new_size, sizeof(size_t));
                            if (ptr == NULL) {
                                message = "Error: could not allocate memory\n";
                                length = my_strlen(message);
                                my_write(2, message, length);
                                close(fd);
                                rc = 1;
                                goto cleanup_and_return;
                            }
                            all_lines_lengths = (size_t *) ptr;
                            all_lines_alloc = new_size;
                        }
                    }
                    all_lines[size_of_all_lines] = current_line;
                    all_lines_lengths[size_of_all_lines] = current_line_size;
                    size_of_all_lines++;
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
                    message = "Error: could not allocate memory\n";
                    length = my_strlen(message);
                    my_write(2, message, length);
                    close(fd);
                    rc = 1;
                    goto cleanup_and_return;
                }
                all_lines = (char **) ptr;
                ptr = calloc(all_lines_alloc, sizeof(size_t));
                if (ptr == NULL) {
                    message = "Error: could not allocate memory\n";
                    length = my_strlen(message);
                    my_write(2, message, length);
                    close(fd);
                    rc = 1;
                    goto cleanup_and_return;
                }
                all_lines_lengths = (size_t *) ptr;
            } else {
                if (size_of_all_lines >= all_lines_alloc) {
                    new_size = all_lines_alloc + all_lines_alloc;
                    if (new_size < all_lines_alloc) {
                        message = "Error: cannot handle memory that large\n";
                        length = my_strlen(message);
                        my_write(2, message, length);
                        close(fd);
                        rc = 1;
                        goto cleanup_and_return;
                    }
                    ptr = reallocarray(all_lines, new_size, sizeof(char *));
                    if (ptr == NULL) {
                        message = "Error: could not allocate memory\n";
                        length = my_strlen(message);
                        my_write(2, message, length);
                        close(fd);
                        rc = 1;
                        goto cleanup_and_return;   
                    }
                    all_lines = (char **) ptr;
                    ptr = reallocarray(all_lines_lengths, new_size, sizeof(size_t));
                    if (ptr == NULL) {
                        message = "Error: could not allocate memory\n";
                        length = my_strlen(message);
                        my_write(2, message, length);
                        close(fd);
                        rc = 1;
                        goto cleanup_and_return;
                    }
                    all_lines_lengths = (size_t *) ptr;
                    all_lines_alloc = new_size;
                }
            }
            all_lines[size_of_all_lines] = current_line;
            all_lines_lengths[size_of_all_lines] = current_line_size;
            size_of_all_lines++;
            current_line = NULL;
            current_line_size = (size_t) 0;
            current_line_alloc = (size_t) 0;
        }
        /*
         * You are suppose to have the lines in the array of pointers
         * now you need to print them. 
         **/

        for(i = (size_t) 0; i < 10; i++){
            if (my_write(1, all_lines[i], all_lines_lengths[i]) < ((size_t)0)){
                message = "Error with my_write(): %s\n";
                length = my_strlen(message);
                my_write(2, message, length);
                close(fd);
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
                reading_result = read_until_newline(0, buf, sizeof(buf));
                if (reading_result < ((ssize_t) 0)){
                    message = "Error with read_until_newline()\n";
                    length = my_strlen(message);
                    my_write(2, message, length);
                    close(fd);
                    rc = 1;
                    goto cleanup_and_return;
                }
                if (reading_result == ((ssize_t) 0)){
                    /*we have hit the EOF condition*/
                    break;
                }
                bytes_read_in = reading_result;
                /*call write*/
                my_write(1, buf, bytes_read_in);
            }

        } else {
            message = "Error with input format\n";
            length = my_strlen(message);
            my_write(2, message, length);
            rc = 1;
            goto cleanup_and_return;
        }

    } else if (argc == 4){
        /* check for case: 1. ./head filename -n # */
        char n[] = "-n";
        char *filename;
        /* case when -n goes the third argument */
        if (isSame(argv[2], n)){
            numberOfLines = my_atoi(argv[3]);
            filename = argv[1];

        } else if (isSame(argv[1], n)){
            /* case when ./head -n # filename  */  
            numberOfLines = my_atoi(argv[2]);
            filename = argv[3];
        } else { 
            message = "Error: write a reasonable order.\n";
            length = my_strlen(message);
            my_write(2, message, length);
            rc = 1;
            goto cleanup_and_return;
        }
        fd = open(filename, O_RDONLY);
        if ( fd < ((ssize_t) 0)){
            /*Error: something happend openning the file*/
            message = "Error: could not open file \n";
            length = my_strlen(message);
            my_write(2, message, length);
            close(fd);
            rc = 1;
            goto cleanup_and_return;
        }
        /* Instructions read from file  the number of lines previously defined*/
        if ( fd < ((ssize_t) 0)){
            /*Error: something happend openning the file*/
            message = "Error: could not open file \n";
            length = my_strlen(message);
            my_write(2, message, length);
            close(fd);
            rc = 1;
            goto cleanup_and_return;
        }
        if (numberOfLines < 0){
            message = "Cannot handle negative input\n";
            length = my_strlen(message);
            my_write(2, message, length);
            close(fd);
            rc = 1;
            goto cleanup_and_return;
        }
        /** 
         * we are reading while there is still content in the file
         * now we need to process what was store in the buffer
         * 
        */
            while (1){
                reading_result = read(fd, buf, sizeof(buf));
                if (reading_result < ((size_t) 0)){
                    message = "Error with read()\n";
                    length = my_strlen(message);
                    my_write(2, message, length);
                    close(fd);
                    rc = 1;
                    goto cleanup_and_return;
                }
                if (reading_result == ((size_t) 0)){
                    break;
                }
                if ( size_of_all_lines == numberOfLines){
                    break;
                }
                bytes_read_in = (size_t) reading_result;
                for(i = (size_t) 0; i < bytes_read_in; i++){
                    byte = buf[i];


                    if (current_line == NULL){
                        current_line_alloc = (size_t) 1;
                        ptr = calloc(current_line_alloc, sizeof(char));
                        if (ptr == NULL){
                            message = "Error: could not allocate memory\n";
                            length = my_strlen(message);
                            my_write(2, message, length);
                            close(fd);
                            rc = 1;
                            goto cleanup_and_return;
                        }
                        current_line = (char *) ptr;
                    } else {

                        if (current_line_size >= current_line_alloc){

                            new_size = current_line_alloc + current_line_alloc;
                            if (new_size < current_line_alloc){
                                message = "Error: cannot handle memory that large\n";
                                length = my_strlen(message);
                                my_write(2, message, length);
                                close(fd);
                                rc = 1;
                                goto cleanup_and_return;
                            }
                            ptr = reallocarray(current_line, new_size, sizeof(char));
                            if (ptr == NULL){
                                message = "Error: could not allocate memory\n";
                                length = my_strlen(message);
                                my_write(2, message, length);
                                close(fd);
                                rc = 1;
                                goto cleanup_and_return;
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
                                message = "Error: could not allocate memory\n";
                                length = my_strlen(message);
                                my_write(2, message, length);
                                close(fd);
                                rc = 1;
                                goto cleanup_and_return;
                            }
                            all_lines = (char **) ptr;
                            ptr = calloc(all_lines_alloc, sizeof(size_t));
                            if (ptr == NULL){
                                message = "Error: could not allocate memory\n";
                                length = my_strlen(message);
                                my_write(2, message, length);
                                close(fd);
                                rc = 1;
                                goto cleanup_and_return;
                            }
                            all_lines_lengths = (size_t *)  ptr;
                        } else {
                            if (size_of_all_lines >= all_lines_alloc) {
                                new_size = all_lines_alloc + all_lines_alloc;
                                if (new_size < all_lines_alloc) {
                                    message = "Error: cannot handle memory that large\n";
                                    length = my_strlen(message);
                                    my_write(2, message, length);
                                    close(fd);
                                    rc = 1;
                                    goto cleanup_and_return;
                                }
                                ptr = reallocarray(all_lines, new_size, sizeof(char *));
                                if (ptr == NULL) {
                                    message = "Error: could not allocate memory\n";
                                    length = my_strlen(message);
                                    my_write(2, message, length);
                                    close(fd);
                                    rc = 1;
                                    goto cleanup_and_return;
                                }
                                all_lines = (char **) ptr;
                                ptr = reallocarray(all_lines_lengths, new_size, sizeof(size_t));
                                if (ptr == NULL) {
                                    message = "Error: could not allocate memory\n";
                                    length = my_strlen(message);
                                    my_write(2, message, length);
                                    close(fd);
                                    rc = 1;
                                    goto cleanup_and_return;
                                }
                                all_lines_lengths = (size_t *) ptr;
                                all_lines_alloc = new_size;
                            }
                        }
                        all_lines[size_of_all_lines] = current_line;
                        all_lines_lengths[size_of_all_lines] = current_line_size;
                        size_of_all_lines++;
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
                        message = "Error: could not allocate memory\n";
                        length = my_strlen(message);
                        my_write(2, message, length);
                        close(fd);
                        rc = 1;
                        goto cleanup_and_return;
                    }
                    all_lines = (char **) ptr;
                    ptr = calloc(all_lines_alloc, sizeof(size_t));
                    if (ptr == NULL) {
                        message = "Error: could not allocate memory\n";
                        length = my_strlen(message);
                        my_write(2, message, length);
                        close(fd);
                        rc = 1;
                        goto cleanup_and_return;
                    }
                    all_lines_lengths = (size_t *) ptr;
                } else {
                    if (size_of_all_lines >= all_lines_alloc) {
                        new_size = all_lines_alloc + all_lines_alloc;
                        if (new_size < all_lines_alloc) {
                            message = "Error: cannot handle memory that large\n";
                            length = my_strlen(message);
                            my_write(2, message, length);
                            close(fd);
                            rc = 1;
                            goto cleanup_and_return;
                        }
                        ptr = reallocarray(all_lines, new_size, sizeof(char *));
                        if (ptr == NULL) {
                            message = "Error: could not allocate memory\n";
                            length = my_strlen(message);
                            my_write(2, message, length);
                            close(fd);
                            rc = 1;
                            goto cleanup_and_return;
                        }
                        all_lines = (char **) ptr;
                        ptr = reallocarray(all_lines_lengths, new_size, sizeof(size_t));
                        if (ptr == NULL) {
                            message = "Error: could not allocate memory\n";
                            length = my_strlen(message);
                            my_write(2, message, length);
                            close(fd);
                            rc = 1;
                            goto cleanup_and_return;
                        }
                        all_lines_lengths = (size_t *) ptr;
                        all_lines_alloc = new_size;
                    }
                }
                all_lines[size_of_all_lines] = current_line;
                all_lines_lengths[size_of_all_lines] = current_line_size;
                size_of_all_lines++;
                current_line = NULL;
                current_line_size = (size_t) 0;
                current_line_alloc = (size_t) 0;
            }
            /*
            * You are suppose to have the lines in the array of pointers
            * now you need to print them. 
            **/
            for(i= (size_t) 0; i < numberOfLines; i++){
                if (my_write(1, all_lines[i], all_lines_lengths[i]) < ((size_t)0)){
                        message = "Error with my_write()\n";
                        length = my_strlen(message);
                        my_write(2, message, length);
                        close(fd);
                        rc = 1;
                        goto cleanup_and_return;
                }
            }
    }


cleanup_and_return:
    if (current_line !=NULL){
        free(current_line);
    }
    if (all_lines != NULL){
        for (i=(size_t)0; i < size_of_all_lines; i++){
            free(all_lines[i]);
        }
        free(all_lines);
        free(all_lines_lengths);
    }
    return rc;
}


/*writes to the screen all that is in buf*/
ssize_t my_write(int fd, const char *buf, size_t count){
    int length;
    char *message;
    size_t total_written = 0;
    ssize_t bytes_written;
    if (count == ((size_t) 0)) return (ssize_t) 0;
    while (total_written < count){
        bytes_written = write(fd, buf + total_written, count - total_written);
        if (bytes_written < ((size_t) 0)) {
            message = "Error with write()\n";
            length = my_strlen(message);
            my_write(2, message, length);
            close(fd);
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
    int length;
    char *message;
    ssize_t total_read = 0;
    char ch;
    while (total_read < max_size){
        ssize_t bytes_read = read(fd, &ch, 1);
        if (bytes_read < ((size_t) 0)){
            message = "Error with read()\n";
            length = my_strlen(message);
            my_write(2, message, length);
            close(fd);
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
    char *message;
    int i, n, sign, length;
    i=0;
    sign = (s[i]== '-')? -1 : 1;
    if (s[i]=='+' || s[i]=='-')
        i++;

    for (n=0; s[i]!='\0'; i++){
        if (!isDigit(s[i])){
            message = "Error: it is not a valid digit\n";
            length = my_strlen(message);
            my_write(2, message, length);
            return -1;
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
