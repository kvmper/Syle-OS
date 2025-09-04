#ifndef PRINT_H
#define PRINT_H

#include <stdint.h>

extern int cursor_pos;

void print_char(char c, uint8_t colorbg, uint8_t colorfg);
void print(const char* str, uint8_t colorbg, uint8_t colorfg);
void set_cursor(int cursor_pos);

#endif
