
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

typedef enum {
    META_COMMAND_SUCCESS,
    META_COMMAND_RECOGNIZED_COMMAND
} MetaCommandResult;


// Defining a InputBuffer for the REPL
typedef struct {
    char* buffer;
    size_t buffer_length;
    ssize_t input_length;    
} InputBuffer;

// Assigning memory to the input buffer
InputBuffer* new_input_buffer() {
    InputBuffer* input_buffer = malloc(sizeof(InputBuffer));
    input_buffer->buffer = NULL;
    input_buffer->buffer_length = 0;
    input_buffer->input_length = 0;

    return input_buffer;
}

// Reading input
void read_input(InputBuffer *buf) {
    ssize_t bytes_read = getline(&(buf->buffer), &(buf->buffer_length), stdin);

    if (bytes_read <= 0) { // invalud input
        printf("Error reading input\n");
        exit(EXIT_FAILURE);
    }

    buf->input_length = bytes_read - 1;
    
    // ignoring trailing new line
    buf->buffer[bytes_read - 1] = 0;
}

// closing the input buffer
void close_input_buffer(InputBuffer *buf) {
    free(buf->buffer);
    free(buf);
}

// default prompt for the repl
void print_prompt() {
    printf("db > ");
}

/*
Note: For the REPL to work we need a never ending loop with prompts
And ability to read and process input at each step

We will also need to handle .exit, similar to sqllite to escape from the prompt
*/

int main(int argc, char* argv[]) {
    InputBuffer* buf = new_input_buffer();
    while (true) {
        print_prompt(); // this is the default prompt
        read_input(buf); 

        // checking for special conditions
        if (strcmp(buf->buffer, ".exit") == 0) {
            // the user intended to exit 
            close_input_buffer(buf);
            exit(EXIT_SUCCESS);
        } else{
            printf("Unrecognized Command '%s'. \n", buf->buffer);
        }
    }
}