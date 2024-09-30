/*Cesar Alvarez
 *Ricardo Acosta
 *Homework 1
*/

#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#define DEFAULT_LINE_COUNT 10
#define BUFFER_SIZE 4096

//Display error message
void display_error(const char *operation, const char *filename){
    write(STDERR_FILENO, "An error occured during ",25);
    write(STDERR_FILENO, operation, strlen(operation)); //Write the specific operation when error occurs
    if(filename && strlen(filename) > 0){ //If filename is provided, it is included in the error message
        write(STDERR_FILENO, ": ", 2);
        write(STDERR_FILENO, filename, strlen(filename));
    }
    write(STDERR_FILENO, "\n", 1);
}


int read_from_pipe(int fd, int num_lines) {
    char buffer[BUFFER_SIZE];
    int line_count = 0;
    int read_size;
    int total_size = 0;
    char *file_content = NULL;
    
    // Read the entire input into memory
    while ((read_size = read(fd, buffer, BUFFER_SIZE)) > 0) {
        file_content = realloc(file_content, total_size + read_size);
        if (!file_content) {
            display_error("memory allocation", "");
            return -1;
        }
        memcpy(file_content + total_size, buffer, read_size);
        total_size += read_size;
    }
    //If read failed
    if (read_size == -1) {
        free(file_content);
        display_error("reading input", "");
        return -1;
    }
    //Process content from end to get last lines
    for (int i = total_size - 1; i >= 0 && line_count < num_lines; i--) {
        if (file_content[i] == '\n') {
            line_count++;
        }
    }
    //Print the last num_lines
    int start_pos = total_size;
    for (int i = 0; i < total_size && line_count >= 0; i++) {
        if (file_content[i] == '\n') {
            line_count--;
            start_pos = i + 1;
            if (line_count == 0) {
                break;
            }
        }
    }
    write(STDOUT_FILENO, file_content + start_pos, total_size - start_pos);
    free(file_content);
    return 0;
}

int read_last_lines(int fd, int num_lines) {
    char buffer[BUFFER_SIZE];
    int line_count = 0;
    int read_size;
    off_t file_position;
    int i;
    file_position = lseek(fd, 0, SEEK_END); //Get the file size and move to the end
    if (file_position == -1) {
        display_error("lseek", "end of file");
        return -1;
    }
    //Start reading backward to count lines
    while (file_position > 0 && line_count < num_lines) {
        int chunk_size = file_position < BUFFER_SIZE ? file_position : BUFFER_SIZE;
        file_position -= chunk_size;
        //Move the file pointer backward and read
        if (lseek(fd, file_position, SEEK_SET) == -1 || (read_size = read(fd, buffer, chunk_size)) == -1) {
            display_error("reading file", "backward");
            return -1;
        }
        //Count the number of newlines to determine the lines encountered
        for (i = read_size - 1; i >= 0 && line_count < num_lines; i--) {
            if (buffer[i] == '\n') {
                line_count++;
            }
        }
    }
    //Ensure the file pointer is positioned correctly for the final output
    if (file_position == 0 && line_count < num_lines) {
        lseek(fd, 0, SEEK_SET);  //If needed to print from start of line
    } else {
        lseek(fd, file_position + i + 2, SEEK_SET);  //Adjust to start of desired content
    }
    //Print the remaining content 
    while ((read_size = read(fd, buffer, BUFFER_SIZE)) > 0) {
        write(STDOUT_FILENO, buffer, read_size);
    }

    return 0;
}


int main(int argc, char *argv[]) {
    int lines_to_print = DEFAULT_LINE_COUNT;
    int file_index = -1;
    int fd;
    struct stat file_info;

    //Parse arguments for -n option and filename
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-n") == 0 && i + 1 < argc) {
            lines_to_print = atoi(argv[++i]);
            if (lines_to_print <= 0) {
                display_error("invalid line count", "");
                return 1;
            }
        } else if (argv[i][0] != '-') {
            file_index = i;
        }
    }
    //Open file if specified
    if (file_index != -1) {
        fd = open(argv[file_index], O_RDONLY);
        if (fd == -1) {
            display_error("opening file", argv[file_index]);
            return 1;
        }
    } else {
        fd = STDIN_FILENO;
    }
    //Check if fd is a regular file or a pipe
    if (fstat(fd, &file_info) == -1) {
        display_error("fstat", "");
        return 1;
    }
    if (S_ISREG(file_info.st_mode)) {
        //Regular file
        if (read_last_lines(fd, lines_to_print) == -1) {
            if (file_index != -1) close(fd);
            return 1;
        }
    } else {
        //Pipe 
        if (read_from_pipe(fd, lines_to_print) == -1) {
            if (file_index != -1) close(fd);
            return 1;
        }
    }
    //Close file
    if (file_index != -1) close(fd);

    return 0;
}