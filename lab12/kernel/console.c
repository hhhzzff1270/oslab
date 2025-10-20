
#include "printf.h"

extern void uart_putc(char c);

void console_putc(char c) {
    uart_putc(c); 
}

void clear_screen(void) {
    console_puts("\033[2J\033[H");
}

void goto_xy(int x, int y) {
    printf("\033[%d;%dH", y + 1, x + 1); 
}

int printf_color(int color, const char *fmt, ...) {
    printf("\033[%dm", color);
    printf("\033[0m");
    return 0;
}