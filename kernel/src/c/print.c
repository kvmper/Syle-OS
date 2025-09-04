#include <stdint.h>
#include "print.h"
#include "video.h"

volatile unsigned short* video = (unsigned short*)0xB8000;

void print_char(char c, uint8_t colorbg, uint8_t colorfg) {
    if (c == '\n') {
        cursor_pos += WIDTH - (cursor_pos % WIDTH);
    } else if (c == '\b') {
        if (cursor_pos > 10) {
            cursor_pos--;
            video[cursor_pos] = (' ' | (WHITE_ON_BLUE << 8));
        }
    } else {
        video[cursor_pos] = ((colorbg << 4 | colorfg) << 8) | c;
        cursor_pos++;
    }
    set_cursor(cursor_pos);
}

void print(const char* str, uint8_t colorbg, uint8_t colorfg) {
    while (*str != '\0') {
        print_char(*str, colorbg, colorfg);
        str++;
    }
}
