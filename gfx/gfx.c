#include <limine.h>
#include <stdint.h>
#include <stddef.h>

void draw_window(void) {
  // Define the parameters for the square.
    int x = 300;
    int y = 300;
    int size = 100;

    // Draw the square.
    for (int i = 0; i < size; i++) {
        uint32_t *fb_ptr = framebuffer->address;
        int row = y + i;
        int col_start = x;
        int col_end = x + size - 1;
        int col_count = col_end - col_start + 1;
        int fb_pitch = framebuffer->pitch / 4;
        uint32_t color = 0xFFFFFF; // White color.

        for (int j = 0; j < col_count; j++) {
            *(fb_ptr + row * fb_pitch + col_start + j) = color;
        }
    }
}
