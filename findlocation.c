#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <stdlib.h>
#define ROW_SIZE 32
#define BUFFER_SIZE 4096
/*Function declaration*/
int isDigit(char c);
int my_atoi(char s[]);
ssize_t my_write(int fd, const char *buf, size_t count);
int my_strlen(char c[]);
int get_number(char* line);
void get_location(char *line, char *current_location);
int display_error_message(const char *buf, size_t count);
/**
 * Missing items: handle the case when lseek fails on the second argument (file)
*/

int main(int argc, char **argv){
    int target, rc, fd, length;
    rc = 0;
    char *message, *filename, *current_line, byte;
    int found = 0;
    char **all_lines;
    char buf[BUFFER_SIZE];
    ssize_t reading_result;
    size_t bytes_read, i;
    size_t current_line_size, current_line_alloc, new_size;
    size_t size_of_all_lines, all_lines_alloc;
    void *ptr;
    size_t *all_lines_lengths;
    filename = "nanpa.txt";
    if (argc == 1){
        /*tool must fail and display a usage message on standard error*/
        message = "Error: not enough arguments\nUsage: ./findlocation  <number> or ./findlocation  <number> <filename>\n";
        length = my_strlen(message);
        display_error_message(message, length);
        rc = 1;
        found = 1;
        goto end;
    } else if (argc == 2){
        /*check that the argument is a valid number and if so we have the number in the target variable*/
        if ((target = my_atoi(argv[1]))<1){
            message = "Error: invalid number, please enter only digits.\n";
            length = my_strlen(message);
            display_error_message(message, length);
            rc = 1;
            found = 1;
            goto end;
        }
        fd = open(filename, O_RDONLY);
        if (fd < ((size_t) 0)){
            message = "Error: could not open file\n";
            length = my_strlen(message);
            display_error_message(message, length);
            rc = 1;
            goto end;
        }
        /**File is open now it is using mmap()*/
        off_t file_size = lseek(fd, 0, SEEK_END);
        if (file_size < ((size_t) 0)){
            current_line = NULL;
            current_line_size = (size_t) 0;
            current_line_alloc = (size_t) 0;
            all_lines = NULL;
            all_lines_lengths = NULL;
            size_of_all_lines = (size_t) 0;
            all_lines_alloc = (size_t) 0;
            fd = open(filename, O_RDONLY);
            if ( fd < ((ssize_t) 0)){
                /*Error: something happend openning the file*/
                message = "Error: could not open file \n";
                length = my_strlen(message);
                my_write(2, message, length);
                close(fd);
                rc = 1;
                goto end;
            }
            /*Reading the file manually since lseek fail*/

            while (1){
                reading_result = read(fd, buf, sizeof(buf));
                if (reading_result < ((size_t) 0)){
                    message = "Error: an error ocurr with read()\n";
                    length = my_strlen(message);
                    my_write(2, message, length);
                    close(fd);
                    rc = 1;
                    goto end;
                }
                if (reading_result == ((size_t) 0)){
                    break;
                }
                if ( size_of_all_lines == 10){
                    break;
                }
                bytes_read = (size_t) reading_result;
                for(i = (size_t) 0; i < bytes_read; i++){
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
                            goto end;
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
                                goto end;
                            }
                            ptr = reallocarray(current_line, new_size, sizeof(char));
                            if (ptr == NULL){
                                message = "Error: could not allocate memory\n";
                                length = my_strlen(message);
                                my_write(2, message, length);
                                close(fd);
                                rc = 1;
                                goto end;
                                
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
                                goto end;
                            }
                            all_lines = (char **) ptr;
                            ptr = calloc(all_lines_alloc, sizeof(size_t));
                            if (ptr == NULL){
                                message = "Error: could not allocate memory\n";
                                length = my_strlen(message);
                                my_write(2, message, length);
                                close(fd);
                                rc = 1;
                                goto end;
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
                                    goto end;
                                }
                                ptr = reallocarray(all_lines, new_size, sizeof(char *));
                                if (ptr == NULL) {
                                    message = "Error: could not allocate memory\n";
                                    length = my_strlen(message);
                                    my_write(2, message, length);
                                    close(fd);
                                    rc = 1;
                                    goto end;
                                }
                                all_lines = (char **) ptr;
                                ptr = reallocarray(all_lines_lengths, new_size, sizeof(size_t));
                                if (ptr == NULL) {
                                    message = "Error: could not allocate memory\n";
                                    length = my_strlen(message);
                                    my_write(2, message, length);
                                    close(fd);
                                    rc = 1;
                                    goto end;
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
                        goto end;
                    }
                    all_lines = (char **) ptr;
                    ptr = calloc(all_lines_alloc, sizeof(size_t));
                    if (ptr == NULL) {
                        message = "Error: could not allocate memory\n";
                        length = my_strlen(message);
                        my_write(2, message, length);
                        close(fd);
                        rc = 1;
                        goto end;
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
                            goto end;
                        }
                        ptr = reallocarray(all_lines, new_size, sizeof(char *));
                        if (ptr == NULL) {
                            message = "Error: could not allocate memory\n";
                            length = my_strlen(message);
                            my_write(2, message, length);
                            close(fd);
                            rc = 1;
                            goto end;   
                        }
                        all_lines = (char **) ptr;
                        ptr = reallocarray(all_lines_lengths, new_size, sizeof(size_t));
                        if (ptr == NULL) {
                            message = "Error: could not allocate memory\n";
                            length = my_strlen(message);
                            my_write(2, message, length);
                            close(fd);
                            rc = 1;
                            goto end;
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
            /**we have the file in the array, we need to perform binary binary search in the array*/
            int low, high, middle, current_number;
            char current_location[25];
            low = 0; high = size_of_all_lines-1;
            while (low <= high){    
                middle = (low+high)/2;
                current_number = get_number(all_lines[middle]);
                if (current_number == target){
                    get_location(all_lines[middle], current_location);
                    message = "The location is: ";
                    my_write(1, message, my_strlen(message));
                    my_write(1, current_location, my_strlen(current_location));
                    found = 1;
                    break;
                } else if (target < current_number){
                    high = (middle - 1);
                } else {
                    low = (middle + 1);
                }
            }
            /**we need to free up the memory use in the reading of file*/
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

        } else{
            char *mappedFile = mmap(NULL, file_size,  PROT_READ, MAP_PRIVATE,fd, 0);
            if (mappedFile == MAP_FAILED){
                message = "Error: could not store the file using mmap\n";
                length = my_strlen(message);
                display_error_message(message, length);
                close(fd);
                rc = 1;
                goto end;
            }
            /**performs binary search*/
            int low, high, middle, current_number;
            char current_location[25];
            low = 0; high = (file_size-ROW_SIZE);
            while (low <= high){    
                middle = (low+high)/2;
                middle = (middle/ROW_SIZE) * ROW_SIZE;
                current_number = get_number(&mappedFile[middle]);
                if (current_number == target){
                    get_location(&mappedFile[middle], current_location);
                    message = "The location is: ";
                    my_write(1, message, my_strlen(message));
                    my_write(1, current_location, my_strlen(current_location));
                    found = 1;
                    break;
                } else if (target < current_number){
                    high = (middle - ROW_SIZE);
                } else {
                    low = (middle + ROW_SIZE);
                }
            }
            /**frees memory used for the mapping*/
            if (munmap(mappedFile, file_size) < ((size_t) 0)){
                message = "Error: an error occurr while using munmap\n";
                length = my_strlen(message);
                display_error_message(message, length);
                close(fd);
                rc = 1;
                goto end;
            }
            close(fd);
        }
        
    } else if (argc == 3){
        /**
         * Open the file and repeat the same process as with only the number.
        */
        if ((target = my_atoi(argv[1]))<1){
            message = "Error: invalid number, please enter only digits.\n";
            length = my_strlen(message);
            display_error_message(message, length);
            rc = 1;
            goto end;
        }
        filename = argv[2];
        fd = open(filename, O_RDONLY);
        if (fd < ((size_t) 0)){
            message = "Error: could not open file\n";
            length = my_strlen(message);
            display_error_message(message, length);
            rc = 1;
            goto end;
        }
        /**Gets the file size by using SEEL_END*/
        off_t file_size = lseek(fd, 0, SEEK_END);
        if (file_size < ((size_t) 0)){
            message = "Error: could not get the file size\n";
            length = my_strlen(message);
            display_error_message(message, length);
            rc = 1;
            goto end;
        }
        char *mappedFile = mmap(NULL, file_size,  PROT_READ, MAP_PRIVATE,fd, 0);
        if (mappedFile == MAP_FAILED){
            message = "Error: could not store the file using mmap\n";
            length = my_strlen(message);
            display_error_message(message, length);
            close(fd);
            rc = 1;
            goto end;
        }
        /**performs binary search*/
        int low, high, middle, current_number;
        char current_location[25];
        low = 0; high = (file_size-ROW_SIZE);
        while (low <= high){    
            middle = (low+high)/2;
            middle = (middle/ROW_SIZE) * ROW_SIZE;
            current_number = get_number(&mappedFile[middle]);
            if (current_number == target){
                get_location(&mappedFile[middle], current_location);
                message = "The location is: ";
                my_write(1, message, my_strlen(message));
                my_write(1, current_location, my_strlen(current_location));
                found = 1;
                break;
            } else if (target < current_number){
                high = (middle - ROW_SIZE);
            } else {
                low = (middle + ROW_SIZE);
            }
        }
        if (munmap(mappedFile, file_size) < ((size_t) 0)){
            message = "Error: an error occurr while using munmap\n";
            length = my_strlen(message);
            display_error_message(message, length);
            close(fd);
            rc = 1;
            goto end;
        }
        close(fd);
    }
end:
    if (!found){
        message = "Number was not found\n";
        length = my_strlen(message);
        my_write(1, message, length);
    }
    return rc;
}

/**
 * Converts a string to int
 * @return 1 if it is not a digit and 0 if it is a digit
*/
int my_atoi(char s[]){
    int i, n, sign;
    i=0;
    sign = (s[i]== '-')? -1 : 1;
    if (s[i]=='+' || s[i]=='-')
        i++;

    for (n=0; s[i]!='\0'; i++){
        if (!isDigit(s[i])){
            return -1;
        }
        n = 10 * n + (s[i] - '0');
    }
    return sign*n;
}
/*checks if c is a digit */
int isDigit(char c){
    return '0' <= c  && c <='9';
}


/**
 * better implementation of write
 * Writes to standard output
 * */
ssize_t my_write(int fd, const char *buf, size_t count){
    size_t total_written = 0;
    ssize_t bytes_written;
    if (count == ((size_t) 0)) return (ssize_t) 0;
    while (total_written < count){
        bytes_written = write(fd, buf + total_written, count - total_written);
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
/**gets the size of the string*/
int my_strlen(char c[]){
    int i;
    for(i=0;c[i]!='\0';i++)
        ;
    return i;
}
/*gets the number from the file*/
int get_number(char *line){
    int i, number;
    number = 0;
    for (i= 0; i < 6; i++){
        if (isDigit(line[i])){
            number = number * 10 + (line[i] - '0');
        }
    }
    return number;
}
/*gets teh location form the file*/
void get_location(char *line, char *current_location){
    int i, j;
    for (i = 6, j = 0; i < 31; i++, j++){
        if (line[i] == ' ' && line[i+1] == ' '){
            current_location[j] = '\n';
            current_location[j+1] = '\0';
            break;
        } 
        current_location[j] = line[i];
    }
    return;
}
/**displays error message*/
int display_error_message(const char *buf, size_t count){
    int fd, length;
    fd = 2;
    size_t total_written = 0;
    ssize_t bytes_written;
    char *message;
    if (count == ((size_t) 0)) return (ssize_t) 0;
    while (total_written < count){
        bytes_written = write(fd, buf + total_written, count - total_written);
        if (bytes_written < ((size_t) 0)) {
            message = "An error ocurred while writing";
            length = my_strlen(message);
            my_write(2, message, length);
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