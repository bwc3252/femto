#ifndef LINE_H
#define LINE_H

// Each line in a text file is a node is a doubly-linked list. This is not
// particularly efficient, especially for large files, but should be
// adequate for now. It has the advantage of allowing efficient insertions
// and deletions, when compared with storing the entire file as a string.
struct line_s {
    struct line_s *previous;    // previous line
    struct line_s *next;        // next line
    char *text;                 // character array
    unsigned int max_size;      // size of allocated character array (in bytes)
    unsigned int length;        // current length of line
} line_s;

// typedef for convenience
typedef struct line_s *line_t;

//////////////////////////////////
//                              //
// Constructors and destructors //
//                              //
//////////////////////////////////

// Create a line with text
line_t create_line(char *text);

// Deallocate a line
void destroy_line(line_t line);

/////////////////////
//                 //
// Line operations //
//                 //
/////////////////////

// Insert a line after the given line
void insert_line_after(line_t to_insert, line_t previous);

// Special case -- insert a line as the first line in the list
void insert_line_as_first(line_t to_insert, line_t first);

// Split a line into two lines
void split_line(line_t line, unsigned int col);

// Concatenate next line onto the end of given line
void concatenate_line(line_t line);

// Delete a line
void delete_line(line_t line);

// Insert a character in a line. The given column MUST be in the line.
void insert_character(line_t line, char c, unsigned int col);

// Delete a character from a line. The given column MUST be in the line.
void delete_character(line_t line, unsigned int col);

#endif
