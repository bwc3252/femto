// line functionality

#include <stdlib.h>
#include <string.h>

#include "line.h"

//////////////////////////////////
//                              //
// Constructors and destructors //
//                              //
//////////////////////////////////

// Create a line with text
line_t create_line(char *text) {
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
    char *nextline_text = line->text + col; // add the offset
    line_t next_line = create_line(nextline_text);
    char *new_text = calloc(col + 1, sizeof(char));
    strncpy(new_text, line->text, col); // copy the first (col) characters
    free(line->text); // free memory from the old line's text
    line->text = new_text;
    // put lines in correct order
    next_line->next = line->next;
    line->next = next_line;
    // fix line lengths
    line->length = col;
    next_line->length = strlen(next_line->text);
}

// Concatenate line onto the end of the previous line
void concatenate_line(line_t line) {
    if (line->previous == NULL) {
        return;        
    }
    char *prev_text = line->previous->text;
    // check if there's enough space
    if (line->previous->max_length < (line->previous->length + line->length)) {
        line->previous->text = realloc(prev_text, 
                (line->previous->length + line->length) * sizeof(char));
        line->previous->max_length = line->previous->length + line->length;
    }
    strcpy(prev_text + line->previous->length, line->text);
    line->previous->length = strlen(line->previous->text);
    delete_line(line);
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
    
}

// Insert a character at the end of a line
void insert_character_at_end(line_t line, char c) {
    // check if allocated space is large enough
    if (line->max_length == line->length) {
        line->text = realloc(line->text, 2 * (line->max_length + 1));
        line->max_length = line->max_length + 1;
    }
    line->text[line->length] = c;
    ++ line->length;
    line->text[line->length] = '\0';
}

// Delete a character from a line. The given column MUST be in the line.
void delete_character(line_t line, unsigned int col) {

}
