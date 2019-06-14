// main file for femto editor
//
// author: Ben Champion <bwc3252@rit.edu>

#include <stdio.h>
#include <stdlib.h>

#include "line.h"
#include "buffer.h"

int main(int argc, char **argv) {
    (void)argc;
    const char *filename = argv[1];
    buffer_t buffer = read_into_buffer(filename);
    char *text = get_lines_as_string(buffer, 1, buffer->line_count);
    printf("%s", text);
    free(text);
    save_buffer(buffer);
    destroy_buffer(buffer);
    return 0;
}
