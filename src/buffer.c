// buffer functionality

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "buffer.h"
#include "line.h"

//////////////////////////////////
//                              //
// Constructors and destructors //
//                              //
//////////////////////////////////

// Create an empty buffer
buffer_t create_buffer(const char *name) {
    buffer_t buffer = malloc(sizeof(struct buffer_s));
    buffer->head = NULL;
    buffer->name = calloc(strlen(name) + 1, sizeof(char));
    strcpy(buffer->name, name);
    buffer->line_count = 0;
    return buffer;
}

// Deallocate a buffer
void destroy_buffer(buffer_t buffer) {
    line_t line = buffer->head;
    line_t nextline;
    while (line != NULL) {
        nextline = line->next;
        destroy_line(line);
        line = nextline;
    }
    free(buffer->name);
    free(buffer);
}

///////////////////////
//                   //
// Buffer operations //
//                   //
///////////////////////

// Save the buffer
void save_buffer(buffer_t buffer) {
    // get the contents of the buffer as a string
    char *text = get_lines_as_string(buffer, 1, buffer->line_count);
    FILE *file = fopen(buffer->name, "w");
    fputs(text, file);
    fclose(file);
    free(text);
}

// Read a file into a buffer
buffer_t read_into_buffer(const char *filename) {
    // create an empty buffer
    buffer_t buffer = create_buffer(filename);
    // open the file
    FILE *file = fopen(filename, "r");
    // read the file character-by-character
    char c;
    line_t line;
    if ((c = fgetc(file)) != EOF) {
        buffer->head = create_line();
        line = buffer->head;
        ++ buffer->line_count;
    }
    while (c != EOF) {
        if (c == '\n') {
            //line->next = create_line("");
            insert_line_after(create_line(), line);
            line = line->next;
            ++ buffer->line_count;
        }
        else {
            insert_character_at_end(line, c);
        }
        c = fgetc(file);
    }
    // close the file
    fclose(file);
    // as a result of reading the file into the buffer, there is a blank line
    // at the end that we should get rid of
    if (buffer->line_count > 0) {
        delete_line(line);
        -- buffer->line_count;
    }
    return buffer;
}

// Get all the text contained in a range of lines as a string. If end is out of
// range, will only go to end of buffer. If start is out of range, will return
// an empty string.
char *get_lines_as_string(buffer_t buffer, unsigned int start, unsigned int end) {
    // get a count of characters so we know how large to make the array
    unsigned int char_count = 0;
    unsigned int line_no = 1;
    line_t line = buffer->head;
    // iterate to start of range
    while (line_no < start && line != NULL) {
        ++ line_no;
        line = line->next;
    }
    line_t start_line = line;
    while (line_no <= end && line != NULL) {
        char_count += strlen(line->text) + 1; // add 1 for \n
        ++ line_no;
        line = line->next;
    }
    ++ char_count; // add 1 for \0
    char *ret = calloc(char_count, sizeof(char));
    // start putting characters in ret
    unsigned int ind = 0;
    line_no = start;
    line = start_line; // go back to start of range
    while (line_no <= end && line != NULL) {
        for (int i = 0; i < line->length; ++ i) {
            ret[ind] = line->text[i];
            ++ ind;
        }
        ret[ind] = '\n';
        ++ ind;
        line = line->next;
        ++ line_no;
    }
    ret[ind] = '\0';
    return ret;
}
