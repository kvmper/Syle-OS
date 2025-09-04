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

#include "video.h"
#include "ports.h"
#include "keyboard.h"
#include "print.h"

#define HEIGHT 25
#define WIDTH 80
#define VGA_ADDR 0xB8000
#define WHITE_ON_BLUE 0x1F

#define SHIFT 0x2A

int cursor_pos;
int char_pos;

void set_cursor(int cursor_pos) {
    int row = cursor_pos / WIDTH;
    int column = cursor_pos % WIDTH;

    out(0x3D4, 0x0F);
    out(0x3D5, column);

    out(0x3D4, 0x0E);
    out(0x3D5, row);
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
