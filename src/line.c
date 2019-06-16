// line functionality

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "line.h"

//////////////////////////////////
//                              //
// Constructors and destructors //
//                              //
//////////////////////////////////

// Create a line with text
line_t create_line(void) {
    line_t line = malloc(sizeof(struct line_s));
    line->previous = NULL;
    line->next = NULL;
    line->max_length = 1;
    line->length = 0;
    line->text = calloc(2, sizeof(char));
    return line;
}

// create a line and put text in it
line_t create_line_with_text(char *text) {
    if (strlen(text) == 0) {
        return create_line();
    }
    line_t line = malloc(sizeof(struct line_s));
    line->previous = NULL;
    line->next = NULL;
    line->max_length = strlen(text);
    line->length = line->max_length;
    line->text = calloc(strlen(text) + 1, sizeof(char));
    strcpy(line->text, text);
    return line;
}

// Deallocate a line
void destroy_line(line_t line) {
    free(line->text);
    free(line);
}

/////////////////////
//                 //
// Line operations //
//                 //
/////////////////////

// Insert a line after the given line
void insert_line_after(line_t to_insert, line_t previous) {
    // set next and previous for node we're inserting
    to_insert->previous = previous;
    if (previous != NULL) {
        if (previous->next != NULL) {
            previous->next->previous = to_insert;
            to_insert->next = previous->next;
        }
        else {
            to_insert->next = NULL;
        }
        previous->next = to_insert;
    }
}

// Special case -- insert a line as the first line in the list
void insert_line_as_first(line_t to_insert, line_t first) {
    to_insert->next = first;
    first->previous = to_insert;
}

// Split a line into two lines at the specified column
void split_line(line_t line, unsigned int col) {
    // create a new line
    line_t next_line = create_line_with_text(line->text + col);
    // insert it in linked list
    next_line->previous = line;
    next_line->next = line->next;
    line->next = next_line;
    // put a NULL terminator in the current line's text (to truncate it)
    line->text[col] = '\0';
    line->length = col;
}

// Concatenate line onto the end of the previous line
line_t concatenate_line(line_t line) {
    char *prev_text = line->previous->text;
    // check if there's enough space
    if (line->previous->max_length < (line->previous->length + line->length)) {
        line->previous->text = realloc(prev_text, 
                (line->previous->length + line->length) * sizeof(char));
        line->previous->max_length = line->previous->length + line->length;
    }
    strcpy(prev_text + line->previous->length, line->text);
    line->previous->length = strlen(line->previous->text);
    line_t ret = line->previous;
    delete_line(line);
    return ret;
}

// Delete a line
void delete_line(line_t line) {
    // remove from linked list
    if (line->next != NULL) {
        line->next->previous = line->previous;
    }
    if (line->previous != NULL) {
        line->previous->next = line->next;
    }
    // deallocate
    free(line->text);
    free(line);
}

// Insert a character in a line. The given column MUST be in the line.
void insert_character(line_t line, char c, unsigned int col) {
    // check if allocated space is large enough
    if (line->max_length == line->length) {
        line->text = realloc(line->text, (2 * line->max_length) + 1);
        line->max_length = 2 * line->max_length;
    }
    // shift everything after col to the right by 1
    for (int i = line->length; i >= (int)col; -- i) {
        line->text[i + 1] = line->text[i];
    }
    ++ line->length;
    line->text[col] = c;
}

// Insert a character at the end of a line
void insert_character_at_end(line_t line, char c) {
    // check if allocated space is large enough
    if (line->max_length == line->length) {
        line->text = realloc(line->text, (2 * line->max_length) + 1);
        line->max_length = 2 * line->max_length;
    }
    line->text[line->length] = c;
    ++ line->length;
    line->text[line->length] = '\0';
}

// Delete a character from a line. The given column MUST be in the line.
void delete_character(line_t line, unsigned int col) {
    // shift everything to the right of col over by 1
    for (int i = col; i < line->length; ++ i) {
        line->text[i] = line->text[i + 1];
    }
    -- line->length;
    line->text[line->length] = '\0';
}
