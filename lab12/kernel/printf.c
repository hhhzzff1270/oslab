
#include <stdarg.h>
#include "printf.h"
extern void console_putc(char c);

static void print_number(long num, int base, int sign) {
    char digits[] = "0123456789abcdef";
    char buf[64]; 
    char *ptr = &buf[63]; // 从缓冲区末尾开始填充（逆序）
    *ptr = '\0';
    unsigned long n; // 使用无符号类型来处理所有情况

    if (sign && num < 0) {
        n = -num;       // 转换为正数
        console_putc('-'); // 输出负号
    } else {
        n = num;
    }

    do {
        ptr--;
        *ptr = digits[n % base];
        n /= base;
    } while (n != 0);

    console_puts(ptr);
}

// 输出字符串
void console_puts(const char *s) {
    while (*s) {
        console_putc(*s++);
    }
}

int printf(const char *fmt, ...) {
    va_list ap;
    int state = 0; 
    va_start(ap, fmt);

    for (int i = 0; fmt[i]; i++) {
        char c = fmt[i];
        if (state == 0) {
            if (c == '%') {
                state = 1; 
            } else {
                console_putc(c);
            }
        } else if (state == 1) {
          
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
                    console_putc((char)va_arg(ap, int)); 
                    break;
                case '%':
                    console_putc('%');
                    break;
                default:
                    
                    console_putc('%');
                    console_putc(c);
                    break;
            }
            state = 0; 
        }
    }
    va_end(ap);
    return 0; 
}