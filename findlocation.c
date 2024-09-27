#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/mman.h>
#define ROW_SIZE 32
/*Function declaration*/
int isDigit(char c);
int my_atoi(char s[]);
ssize_t my_write(int fd, const char *buf, size_t count);
int my_strlen(char c[]);
int get_number(char* line);
void get_location(char *line, char *current_location);


int main(int argc, char **argv){
    int target, rc, fd, length;
    rc = 0;
    char *message;
    char *filename;
    filename = "nanpa.txt";
    if (argc == 1){
        /*tool must fail and display a usage message on standard error*/



    } else if (argc == 2){
        /*check that the argument is a valid number and if so we have the number in the target variable*/
        if ((target = my_atoi(argv[1]))<1){
            rc = 1;
            goto end;
        }
        fd = open(filename, O_RDONLY);
        if (fd < ((size_t) 0)){
            message = "Error: could not open file\n";
            length = my_strlen(message);
            my_write(1, message, length);
            rc = 1;
            goto end;
        }
        /**File is open now use mmap()*/
        struct stat struct_block;

        if (fstat(fd, &struct_block)< ((ssize_t) 0)){
            message = "Error: could not get the file size\n";
            length = my_strlen(message);
            my_write(1, message, length);
            close(fd);
            rc = 1;
            goto end;
        }
        size_t filesize = struct_block.st_size;
        char *mappedFile = mmap(NULL, filesize,  PROT_READ, MAP_PRIVATE,fd, 0);
        if (mappedFile == MAP_FAILED){
            message = "Error: could not store the file using mmap\n";
            length = my_strlen(message);
            my_write(1, message, length);
            close(fd);
            rc = 1;
            goto end;
        }
        int low, high, middle, current_number;
        char current_location[25];
        low = 0; high = (filesize-ROW_SIZE);
        while (low <= high){    
            middle = (low+high)/2;
            middle = (middle/ROW_SIZE) * ROW_SIZE;
            current_number = get_number(&mappedFile[middle]);
            printf("Low: %d, High: %d, Middle: %d, Current Number: %d Target: %d\n", low, high, middle, current_number, target);
            // printf("current number - target\n");
            // printf("%d  -   %d\n", current_number, target);
            if (current_number == target){
                get_location(&mappedFile[middle], current_location);
                message = "The location is: ";
                my_write(1, message, my_strlen(message));
                my_write(1, current_location, my_strlen(current_location));
                break;
            } else if (target < current_number){
                high = (middle - ROW_SIZE);
            } else {
                low = (middle + ROW_SIZE);
            }
        }
    }
end:
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

    for (n=0; s[i]!='\0' && i < 6; i++){
        if (!isDigit(s[i])){
            /*rise error*/
            return 1;
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
int my_strlen(char c[]){
    int i;
    for(i=0;c[i]!='\0';i++)
        ;
    return i;
}

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
void get_location(char *line, char *current_location){
    //line = 201200Jersey City NJ           
    // current_location;
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