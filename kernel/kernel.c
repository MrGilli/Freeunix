#include <stdint.h>
#include <stddef.h>
#include <limine.h>

// The Limine requests can be placed anywhere, but it is important that
// the compiler does not optimise them away, so, usually, they should
// be made volatile or equivalent.

static volatile struct limine_framebuffer_request framebuffer_request = {
    .id = LIMINE_FRAMEBUFFER_REQUEST,
    .revision = 0
};

// GCC and Clang reserve the right to generate calls to the following
// 4 functions even if they are not directly called.
// Implement them as the C specification mandates.
// DO NOT remove or rename these functions, or stuff will eventually break!
// They CAN be moved to a different .c file.

void *memcpy(void *dest, const void *src, size_t n) {
    uint8_t *pdest = (uint8_t *)dest;
    const uint8_t *psrc = (const uint8_t *)src;

    for (size_t i = 0; i < n; i++) {
        pdest[i] = psrc[i];
    }

    return dest;
}

void *memset(void *s, int c, size_t n) {
    uint8_t *p = (uint8_t *)s;

    for (size_t i = 0; i < n; i++) {
        p[i] = (uint8_t)c;
    }

    return s;
}

void *memmove(void *dest, const void *src, size_t n) {
    uint8_t *pdest = (uint8_t *)dest;
    const uint8_t *psrc = (const uint8_t *)src;

    if (src > dest) {
        for (size_t i = 0; i < n; i++) {
            pdest[i] = psrc[i];
        }
    } else if (src < dest) {
        for (size_t i = n; i > 0; i--) {
            pdest[i-1] = psrc[i-1];
        }
    }

    return dest;
}

int memcmp(const void *s1, const void *s2, size_t n) {
    const uint8_t *p1 = (const uint8_t *)s1;
    const uint8_t *p2 = (const uint8_t *)s2;

    for (size_t i = 0; i < n; i++) {
        if (p1[i] != p2[i]) {
            return p1[i] < p2[i] ? -1 : 1;
        }
    }

    return 0;
}

// Halt and catch fire function.
static void hcf(void) {
    asm ("cli");
    for (;;) {
        asm ("hlt");
    }
}

// The following will be our kernel's entry point.
// If renaming _start() to something else, make sure to change the
// linker script accordingly.

void main_window(void) {
    // Define the parameters for the square.
    struct limine_framebuffer *framebuffer = framebuffer_request.response->framebuffers[0];
    int x = 0;
    int y = 0;
    int size = 2000;

    // Draw the square.
    for (int i = 0; i < size; i++) {
        uint32_t *fb_ptr = framebuffer->address;
        int row = y + i;
        int col_start = x;
        int col_end = x + size - 1;
        int col_count = col_end - col_start + 1;
        int fb_pitch = framebuffer->pitch / 4;
        uint32_t color = 0x7F7F7F; // White color.

        for (int j = 0; j < col_count; j++) {
            *(fb_ptr + row * fb_pitch + col_start + j) = color;
        }
    }
}

void draw_window(void) {

    // Define the parameters for the square.
    struct limine_framebuffer *framebuffer = framebuffer_request.response->framebuffers[0];

    int x = 100;
    int y = 100;
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


void _start(void) {
    // Ensure we got a framebuffer.
    if (framebuffer_request.response == NULL
     || framebuffer_request.response->framebuffer_count < 1) {
        hcf();
    }

    // Fetch the first framebuffer.
    struct limine_framebuffer *framebuffer = framebuffer_request.response->framebuffers[0];

    // Note: we assume the framebuffer model is RGB with 32-bit pixels.

    main_window();
    draw_window();

    // We're done, just hang...
    hcf();
}
