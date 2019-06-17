// main file for femto editor
//
// author: Ben Champion <bwc3252@rit.edu>

#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>

#include "line.h"
#include "buffer.h"
#include "display.h"

// macro to check for CTRL in front of keypress
#define CTRL(k) ((k)  & 0x1f)

#define ROW_START 2

#define UP          0
#define DOWN        1
#define LEFT        2
#define RIGHT       3
#define NEWLINE     4
#define BACKSPACE   5
#define CHAR        6
#define SAVE        7
#define EXIT        8

// function declarations
int file_exists(char *filename);
void run(buffer_t buffer);

int file_exists(char *filename) {
    FILE *fp;
    if ((fp = fopen(filename, "r")) != NULL) {
        fclose(fp);
        return 1;
    }
    else {
        return 0;
    }
}

void run(buffer_t buffer) {
    view_t view = initialize(buffer);
    line_t line = buffer-> head;
    cursor_t cursor;
    int loop = 1;
    int refresh_display = 1;
    int c;
    while (loop) {
        if (refresh_display) {
            display(view);
        }
        cursor = view->cursor;
        c = wgetch(view->main_window);
        switch(c) {
            case KEY_LEFT:
                if (cursor->abs_col > 0) {
                    -- cursor->abs_col;
                    -- cursor->rel_col;
                }
                break;
            case KEY_RIGHT:
                if (cursor->abs_col < line->length) {
                    ++ cursor->abs_col;
                    ++ cursor->rel_col;
                }
                break;
            case KEY_UP:
                if (line->previous != NULL) {
                    line = line->previous;
                    -- cursor->abs_row;
                    -- cursor->rel_row;
                    // handle case where cursor was moved up a line into 
                    // "emtpy space"
                    if (cursor->abs_col > line->length) {
                        cursor->rel_col -= cursor->abs_col - line->length;
                        cursor->abs_col = line->length;
                    }
                }
                break;
            case KEY_DOWN:
                if (line->next != NULL) {
                    line = line->next;
                    ++ cursor->abs_row;
                    ++ cursor->rel_row;
                    // handle case where cursor was moved down a line into 
                    // "emtpy space"
                    if (cursor->abs_col > line->length) {
                        cursor->rel_col -= cursor->abs_col - line->length;
                        cursor->abs_col = line->length;
                    }
                }
                break;
            case KEY_RESIZE:
                wclear(view->main_window);
                break;
            case 'x':
                loop = 0;
                break;
        }
    }
    destroy_view(view);
    endwin();
}

int main(int argc, char **argv) {
    if (argc < 2) {
        fprintf(stderr, "No filename specified\n");
        return 1;
    }
    char *filename = argv[1];
    buffer_t buffer;
    if (file_exists(filename)) {
        buffer = read_into_buffer(filename);
    }
    else {
        buffer = create_buffer(filename);
        buffer->head = create_line();
        ++ buffer->line_count;
    }
    run(buffer);
    destroy_buffer(buffer);
    return 0;
}
