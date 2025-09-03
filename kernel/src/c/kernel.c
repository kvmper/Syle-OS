__attribute__((section(".multiboot"), used, aligned(4)))
const unsigned int multiboot_header[] = {
    0x1BADB002,
    0x0,
    -(0x1BADB002 + 0x0)
};


/*
__        ___    ____  _   _ ___ _   _  ____ 
\ \      / / \  |  _ \| \ | |_ _| \ | |/ ___|
 \ \ /\ / / _ \ | |_) |  \| || ||  \| | |  _ 
  \ V  V / ___ \|  _ <| |\  || || |\  | |_| |
   \_/\_/_/   \_\_| \_\_| \_|___|_| \_|\____|

    CHANGING ANYTHING MAY BREAK THE OS
    REMEMBER TO RUN MAKE BUILD AFTER CHANGING ANYTHING

*/

/*
 ___ __  __ ____  ____   _____     _______ __  __ _____ _   _ _____ ____  
|_ _|  \/  |  _ \|  _ \ / _ \ \   / / ____|  \/  | ____| \ | |_   _/ ___| 
 | || |\/| | |_) | |_) | | | \ \ / /|  _| | |\/| |  _| |  \| | | | \___ \ 
 | || |  | |  __/|  _ <| |_| |\ V / | |___| |  | | |___| |\  | | |  ___) |
|___|_|  |_|_|   |_| \_\\___/  \_/  |_____|_|  |_|_____|_| \_| |_| |____/ 

To be imrpoved:
- Change keyboard() (when holding shift toggle caps_lock to true and after release make it false)
- Change sleep() (so it actually waits)
- Organize code (split into multiple files)
- Add comments
*/



#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#define HEIGHT 25
#define WIDTH 80
#define VGA_ADDR 0xB8000
#define WHITE_ON_BLUE 0x1F

#define KEYBOARD_STATUS_PORT 0x64
#define KEYBOARD_DATA_PORT 0x60

#define SHIFT 0x2A

volatile unsigned short* video = (unsigned short*)(0xB8000);

int cursor_pos = 0;

int char_pos;

uint8_t in(uint16_t port) {
    uint8_t result;
    asm volatile("inb %1, %0" : "=a"(result) : "Nd"(port));
    return result;
}
void out(uint16_t port, uint8_t value) {
    asm volatile("outb %0, %1" : : "a"(value), "Nd"(port));
}

void set_cursor(int cursor_pos) {
    int row = cursor_pos / WIDTH;
    int column = cursor_pos % WIDTH;

    out(0x3D4, 0x0F);
    out(0x3D5, column);

    out(0x3D4, 0x0E);
    out(0x3D5, row);
}
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

void background() {
    asm volatile (
    "mov $0xB8000, %%edi\n\t"
    "mov $2000, %%ecx\n\t"
    "mov $0x1F20, %%ax\n\t"
    "1:\n\t"
    "mov %%ax, (%%edi)\n\t"
    "add $2, %%edi\n\t"
    "loop 1b\n\t"
    :
    :
    : "ax", "ecx", "edi"
    );
}

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

void __stack_chk_fail_local() { // Do not remove, the compiler will probably give you an error

}

bool caps_lock = false;

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


void sleep(int seconds) {
    int i, j;
    for (i = 0; i < seconds * 1000000; i++) {
        for (j = 0; j < 500; j++) {

        }
    }
}

int strcmp(const char *str1, const char *str2) {
    while (*str1 && (*str1 == *str2)) {
        str1++;
        str2++;
    }
    return (unsigned char)*str1 - (unsigned char)*str2;
}

void clear() {
    for (int i = 0; i < WIDTH * HEIGHT; i++) {
        video[i * 2] = 0;
        video[i * 2 + 1] = WHITE_ON_BLUE;
    }
}

void kernel_main() {
    
    background();
    print("[shell] ?:", 0x1, 0xF);

    while(1) {
        set_cursor(cursor_pos);
        keyboard();
    }
}
