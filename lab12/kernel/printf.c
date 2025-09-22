// printf.c
#include <stdarg.h>
#include "printf.h"
// 假设 console_putc 在其他文件中实现
extern void console_putc(char c);

// 核心数字转换函数：将数字`num`以`base`进制输出，`sign`表示是否为有符号数
static void print_number(long num, int base, int sign) {
    char digits[] = "0123456789abcdef";
    char buf[64]; // 足够存放64位二进制数
    char *ptr = &buf[63]; // 从缓冲区末尾开始填充（逆序）
    *ptr = '\0';
    unsigned long n; // 使用无符号类型来处理所有情况

    if (sign && num < 0) {
        n = -num;       // 转换为正数
        console_putc('-'); // 输出负号
    } else {
        n = num;
    }

    // 使用循环进行转换（避免递归）
    do {
        ptr--;
        *ptr = digits[n % base];
        n /= base;
    } while (n != 0);

    // 输出转换后的字符串
    console_puts(ptr);
}

// 输出字符串
void console_puts(const char *s) {
    while (*s) {
        console_putc(*s++);
    }
}

// 主printf函数
int printf(const char *fmt, ...) {
    va_list ap;
    int state = 0; // 0: normal, 1: after '%'

    va_start(ap, fmt);

    for (int i = 0; fmt[i]; i++) {
        char c = fmt[i];
        if (state == 0) {
            if (c == '%') {
                state = 1; // 进入格式解析状态
            } else {
                console_putc(c); // 输出普通字符
            }
        } else if (state == 1) {
            // 根据格式字符处理
            switch (c) {
                case 'd':
                    print_number(va_arg(ap, int), 10, 1);
                    break;
                case 'u':
                    print_number(va_arg(ap, unsigned int), 10, 0);
                    break;
                case 'x':
                    print_number(va_arg(ap, unsigned int), 16, 0);
                    break;
                case 's': {
                    char *s = va_arg(ap, char *);
                    if (s == 0) {
                        console_puts("(null)");
                    } else {
                        console_puts(s);
                    }
                    break;
                }
                case 'c':
                    console_putc((char)va_arg(ap, int)); // char在可变参数中提升为int
                    break;
                case '%':
                    console_putc('%');
                    break;
                default:
                    // 未知格式符，原样输出%和字符
                    console_putc('%');
                    console_putc(c);
                    break;
            }
            state = 0; // 处理完毕，回归普通状态
        }
    }
    va_end(ap);
    return 0; // 通常返回输出的字符数，这里简化处理
}