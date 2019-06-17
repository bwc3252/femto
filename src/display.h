#include <ncurses.h>

#include "buffer.h"

#ifndef DISPLAY_H
#define DISPLAY_H

// struct to encapsulate information about the cursor
struct cursor_s {
    int abs_row;            // absolute row of the cursor in the document
    int abs_col;            // absolute column of the cursor in the document
    int rel_row;            // cursor row relative to window
    int rel_col;            // cursor column relative to window
} cursor_s;

typedef struct cursor_s *cursor_t;

// struct to encapsulate important information about the text to display
struct view_s {
    int width;              // width of the main display
    int height;             // height of the main display
    int top;                // row for top of display region
    int left;               // column for left side of display region
    buffer_t buffer;        // current buffer
    WINDOW *header;         // header (displays file info)
    WINDOW *main_window;    // main display window
    cursor_t cursor;
} view_s;

typedef struct view_s *view_t;

// initialize the display and header windows and return a view
view_t initialize(buffer_t buffer);

// re-render and display the main and header windows
void display(view_t view);

// free memory for view
void destroy_view(view_t view);

#endif
