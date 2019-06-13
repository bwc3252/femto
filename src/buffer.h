#include "line.h"

#ifndef BUFFER_H
#define BUFFER_H

// A buffer contains a pointer to the first and last nodes in the linked list 
// of lines. It also contains some other information, such as a line count.
struct buffer_s {
    char *name;
    line_t head;
    line_t tail;
    unsigned int line_count;
} buffer_s;

// typedef for convenience
typedef struct buffer_s *buffer_t;

//////////////////////////////////
//                              //
// Constructors and destructors //
//                              //
//////////////////////////////////

// Create an empty buffer
buffer_t new_buffer(char *name);

// Deallocate a buffer
void destroy_buffer(buffer_t buffer);

///////////////////////
//                   //
// Buffer operations //
//                   //
///////////////////////

// Save the buffer
void save_buffer(buffer_t buffer);

// Read a file into a buffer
buffer_t read_into_buffer(char *filename);

#endif
