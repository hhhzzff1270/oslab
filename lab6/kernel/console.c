
#include "printf.h"

extern void uart_putc(char c);

void console_putc(char c) {
    // 未来可以在这里添加缓冲、日志过滤等功能
    uart_putc(c); // 目前直接传递给驱动层
}

void clear_screen(void) {
    console_puts("\033[2J\033[H");
}

void goto_xy(int x, int y) {
    printf("\033[%d;%dH", y + 1, x + 1); // ANSI坐标是1-based
}

int printf_color(int color, const char *fmt, ...) {
    // 设置颜色
    printf("\033[%dm", color);
    printf("\033[0m");
    return 0;
}