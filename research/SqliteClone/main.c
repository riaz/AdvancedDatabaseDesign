
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

typedef enum {
    META_COMMAND_SUCCESS,
    META_COMMAND_UNRECOGNIZED_COMMAND
} MetaCommandResult;

typedef enum { PREPARE_SUCCESS, PREPARE_UNRECOGNIZED_STATEMENT} PrepareResult;

typedef enum { STATEMENT_INSERT, STATEMENT_SELECT} StatementType;

typedef struct {
    StatementType type;
} Statement;

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

// closing the input buffer
void close_input_buffer(InputBuffer *buf) {
    free(buf->buffer);
    free(buf);
}

// This method manages exiting from the REPL using the MetaCommandResult enum
MetaCommandResult do_meta_command(InputBuffer* buf) {
    if (strcmp(buf->buffer, ".exit") == 0) {
        close_input_buffer(buf);
        exit(EXIT_SUCCESS);
    }else{
        return META_COMMAND_UNRECOGNIZED_COMMAND;
    }
}

// This method populates the Statement struct based on the REPL stage
PrepareResult prepare_statement(InputBuffer* buf, Statement* stmt) {
    if (strncmp(buf->buffer, "insert", 6) == 0) {
        stmt->type = STATEMENT_INSERT;
        return PREPARE_SUCCESS;
    }else if (strncmp(buf->buffer, "select", 6) == 0) {
        stmt->type = STATEMENT_SELECT;
        return PREPARE_SUCCESS;
    }
    return PREPARE_UNRECOGNIZED_STATEMENT;
}

// a void executer
void execute_statement(Statement* stmt) {
    switch(stmt->type) {
        case STATEMENT_INSERT:
            printf("This is where we would do an insert\n");
            break;
        case STATEMENT_SELECT:
            printf("This is where we would do a select\n");
            break;
        default:
            printf("Unsupported staetement encountered");
    }
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

        // we will check if a command was passed and try to exeucte it

        // we check if the statement begins with ., and next we validate if its valid
        if (buf->buffer[0] == '.') {
            switch (do_meta_command(buf)) {
                case META_COMMAND_SUCCESS:
                    continue; // all good
                case META_COMMAND_UNRECOGNIZED_COMMAND:
                    printf("Unrecognized Command %s \n", buf->buffer);
                    continue;
            }
        }

        // next we need to initialize statement to register and execute the query
        Statement statement;
        switch (prepare_statement(buf, &statement)) {
            case PREPARE_SUCCESS:
                break;
            case PREPARE_UNRECOGNIZED_STATEMENT:
                printf("Unrecognized Keyword at start %s \n", buf->buffer);
                continue; // we want the repl to continue but not proceed for this iteration
        }

        // now we just execute the statement
        execute_statement(&statement);

        printf("Done Executing\n");
    }
}