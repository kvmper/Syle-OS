#include <stdint.h>
#include "print.h"
#include "ports.h"

#define KEYBOARD_STATUS_PORT 0x64
#define KEYBOARD_DATA_PORT 0x60

#define SHIFT 0x2A

bool caps_lock = false;

char scancodes[128] = {
    [0x0E] = '\b',
    [0x1C] = '\0',
    [0x10] = 'q',
    [0x11] = 'w',
    [0x12] = 'e',
    [0x13] = 'r',
    [0x14] = 't',
    [0x15] = 'y',
    [0x16] = 'u',
    [0x17] = 'i',
    [0x18] = 'o',
    [0x19] = 'p',
    [0x1E] = 'a',
    [0x1F] = 's',
    [0x20] = 'd',
    [0x21] = 'f',
    [0x22] = 'g',
    [0x23] = 'h',
    [0x24] = 'j',
    [0x25] = 'k',
    [0x26] = 'l',
    [0x2C] = 'z',
    [0x2D] = 'x',
    [0x2E] = 'c',
    [0x2F] = 'v',
    [0x30] = 'b',
    [0x31] = 'n',
    [0x32] = 'm',
    [0x39] = ' ',
};

char scancodes_upper[128] = {
    [0x0E] = '\b',
    [0x1C] = '\0',
    [0x10] = 'Q',
    [0x11] = 'W',
    [0x12] = 'E',
    [0x13] = 'R',
    [0x14] = 'T',
    [0x15] = 'Y',
    [0x16] = 'U',
    [0x17] = 'I',
    [0x18] = 'O',
    [0x19] = 'P',
    [0x1E] = 'A',
    [0x1F] = 'A',
    [0x20] = 'D',
    [0x21] = 'F',
    [0x22] = 'G',
    [0x23] = 'H',
    [0x24] = 'J',
    [0x25] = 'K',
    [0x26] = 'L',
    [0x2C] = 'Z',
    [0x2D] = 'X',
    [0x2E] = 'C',
    [0x2F] = 'V',
    [0x30] = 'B',
    [0x31] = 'N',
    [0x32] = 'M',
    [0x39] = ' ',
};

void keyboard() {
    int i = 0;

    if ((in(KEYBOARD_STATUS_PORT) & 0x01) == 0) return;

    uint8_t key_pressed = in(KEYBOARD_DATA_PORT);

    if (key_pressed == 0x3A) {
        caps_lock = !caps_lock;
    }

    if (key_pressed < 128 && scancodes[key_pressed] != 0 && caps_lock == false) {
        char ch = scancodes[key_pressed];
        print_char(ch, 0x1, 0xF);
    } else if(key_pressed < 128 && scancodes[key_pressed] != 0 && caps_lock == true) {
        char ch = scancodes_upper[key_pressed];
        print_char(ch, 0x1, 0xF);
    }
}
