// main file for femto editor
//
// author: Ben Champion <bwc3252@rit.edu>

#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>

#include "line.h"
#include "buffer.h"

// macro to check for CTRL in front of keypress
#define CTRL(k) ((k)  & 0x1f)

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
    int row = 0;
    int col = 0;
    int reprint_buffer;
    line_t line = buffer->head; // TODO this will break for empty buffers
    char *text = get_lines_as_string(buffer, 1, buffer->line_count);
    initscr();
    raw();
    noecho();
    keypad(stdscr, TRUE);
    printw(text);
    free(text);
    // input loop
    int c;
    move(0, 0);
    while (1) {
        reprint_buffer = 0;
        c = getch();
        if (c == KEY_LEFT) {
            if (col > 0) {
                -- col;
            }
        }
        else if (c == KEY_RIGHT) {
            if (col < line->length) {
                ++ col;
            }
        }
        else if (c == KEY_UP) {
            if (row > 0) {
                -- row;
                line = line->previous;
                if (col > line->length) {
                    col = line->length;
                }
            }
        }
        else if (c == KEY_DOWN) {
            if (row < buffer->line_count - 1) {
                ++ row;
                line = line->next;
                if (col > line->length) {
                    col = line->length;
                }
            }
        }
        else if (c == CTRL('s')) {
            save_buffer(buffer);
        }
        else if (c == CTRL('x')) {
            break;
        }
        else if (c == '\n') {
            split_line(line, col);
            line = line->next;
            reprint_buffer = 1;
            ++ buffer->line_count;
            ++ row;
            col = 0;
        }
        else if (c == KEY_BACKSPACE) { // backspace
            if (col == 0) {
                if (row > 0) {
                    col = line->previous->length;
                    line = concatenate_line(line);
                    -- buffer->line_count;
                    -- row;
                }
            }
            else {
                delete_character(line, col - 1);
                -- col;
            }
            reprint_buffer = 1;
        }
        else {
            insert_character(line, c, col);
            ++ col;
            reprint_buffer = 1;
        }
        if (reprint_buffer) {
            clear();
            text = get_lines_as_string(buffer, 1, buffer->line_count);
            mvprintw(0, 0, text);
            free(text);
        }
        move(15, 0);
        printw("row: %d, column: %d", row, col);
        move(row, col);
        refresh();
    }
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
