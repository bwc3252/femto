#include <ncurses.h>
#include <stdlib.h>
#include <string.h>

#include "display.h"
#include "buffer.h"
#include "line.h"

#define HEADER_HEIGHT 2

view_t initialize(buffer_t buffer) {
    // initialize ncurses things
    initscr();
    raw();
    noecho();
    // initialize the view
    int total_width, total_height;
    getmaxyx(stdscr, total_height, total_width);
    view_t view = malloc(sizeof(struct view_s));
    view->width = total_width;
    view->height = total_height - HEADER_HEIGHT;
    view->top = 0;
    view->left = 0;
    view->buffer = buffer;
    view->header = newwin(HEADER_HEIGHT, view->width, 0, 0);
    view->main_window = newwin(view->height, view->width, HEADER_HEIGHT, 0);
    keypad(view->main_window, TRUE);
    // initialize the cursor
    view->cursor = malloc(sizeof(struct cursor_s));
    view->cursor->abs_row = 0;
    view->cursor->abs_col = 0;
    view->cursor->rel_row = 0;
    view->cursor->rel_col = 0;
    // add header text to header
    wattron(view->header, A_BOLD);
    mvwprintw(view->header, 0, 0, buffer->name);
    wattroff(view->header, A_BOLD);
    wrefresh(view->header);
    // move cursor to start position
    wmove(view->main_window, 0, 0);
    return view;
}

void display(view_t view) {
    buffer_t buffer = view->buffer;
    WINDOW *main_window = view->main_window;
    // check if terminal size has changed
    getmaxyx(main_window, view->height, view->width);
    view->height -= 1;
    view->width -= 1;
    // check if cursor has gone out of range
    cursor_t cursor = view->cursor;
    if (cursor->rel_row < 0) {
        cursor->rel_row = 0;
        view->top = cursor->abs_row;
    }
    if (cursor->rel_row > view->height) {
        cursor->rel_row = view->height;
        view->top = cursor->abs_row - view->height;
    }
    if (cursor->rel_col < 0) {
        cursor->rel_col = 0;
        view->left = cursor->abs_col;
    }
    if (cursor->rel_col > view->width) {
        cursor->rel_col = view->width;
        view->left = cursor->abs_col - view->width;
    }
    // get visible text
    char *text = get_lines_as_string(buffer, view->top, view->top + view->height, 
            view->left, view->left + view->width);
    mvwprintw(main_window, 0, 0, text);
    free(text);
    wmove(main_window, cursor->rel_row, cursor->rel_col);
    wrefresh(main_window);
}

void destroy_view(view_t view) {
    free(view->cursor);
    delwin(view->header);
    delwin(view->main_window);
    free(view);
}
