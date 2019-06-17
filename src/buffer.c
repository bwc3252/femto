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
    // find the longest line
    int max_line = 0;
    line_t line = buffer->head;
    while (line != NULL) {
        if (line->length > max_line) {
            max_line = line->length;
        }
        line = line->next;
    }
    // get the contents of the buffer as a string
    char *text = get_lines_as_string(buffer, 0, buffer->line_count, 0, max_line);
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
        else if (c == '\t') {
            for (int i = 0; i < 4; ++ i) {
                insert_character_at_end(line, ' ');
            }
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

static char *reallocate_string(char *str, int *size, int *max_size) {
    if (*size == *max_size) {
        str = realloc(str, 2 * (*max_size) * sizeof(char));
        *max_size *= 2;
    }
    return str;
}

// Get all the text contained in a range of lines as a string. If end is out of
// range, will only go to end of buffer. If start is out of range, will return
// an empty string.
char *get_lines_as_string(buffer_t buffer, int top, int bottom,
        int left, int right) {
    // allocate space for string
    char *str = calloc(1, sizeof(char));
    str[0] = '\0';
    int max_size = 1;
    int size = 0;
    // iterate to first line
    line_t line = buffer->head;
    int i = 0;
    while (i < top && line != NULL) {
        ++ i;
        line = line->next;
    }
    // start filling string
    int k = 0;
    while (i <= bottom && line != NULL) {
        if (left < line->length) {
            for (int j = left; j < line->length && j < right; ++ j) {
                str = reallocate_string(str, &size, &max_size);
                str[k] = line->text[j];
                ++ k;
                ++ size;
            }
        }
        str = reallocate_string(str, &size, &max_size);
        str[k] = '\n';
        ++ k;
        ++ size;
        line = line->next;
        ++ i;
    }
    str = reallocate_string(str, &size, &max_size);
    str[k] = '\0';
    return str;
}
