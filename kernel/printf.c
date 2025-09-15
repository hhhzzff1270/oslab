#include "printf.h"
#include "uart.h"
#include <stdarg.h>

// 数字转换为字符串
static void print_number(int num, int base, int sign) {
    static char digits[] = "0123456789ABCDEF";
    char buf[16];
    int i = 0;
    unsigned int n;
    
    if (sign && num < 0) {
        n = -num;
        uart_putc('-');
    } else {
        n = num;
    }
    
    // 处理0的特殊情况
    if (n == 0) {
        uart_putc('0');
        return;
    }
    
    // 转换为字符串（逆序）
    while (n > 0) {
        buf[i++] = digits[n % base];
        n /= base;
    }
    
    // 逆序输出
    while (--i >= 0) {
        uart_putc(buf[i]);
    }
}

// 清屏功能
void clear_screen(void) {
    // ANSI转义序列：清屏并光标回到左上角
    uart_puts("\033[2J");  // 清除整个屏幕
    uart_puts("\033[H"); 
}

// 格式化输出函数
int printf(const char *fmt, ...) {
    va_list ap;
    const char *p;
    char *s;
    int num;
    char c;
    
    va_start(ap, fmt);
    
    for (p = fmt; *p; p++) {
        if (*p != '%') {
            uart_putc(*p);
            continue;
        }
        
        // 处理格式符
        switch (*++p) {
            case 'd':
                num = va_arg(ap, int);
                print_number(num, 10, 1);
                break;
                
            case 'x':
                num = va_arg(ap, int);
                uart_puts("0x");
                print_number(num, 16, 0);
                break;
                
            case 's':
                s = va_arg(ap, char *);
                if (s == 0) {
                    uart_puts("(null)");
                } else {
                    uart_puts(s);
                }
                break;
                
            case 'c':
                c = (char)va_arg(ap, int);
                uart_putc(c);
                break;
                
            case '%':
                uart_putc('%');
                break;
                
            default:
                uart_putc('%');
                uart_putc(*p);
                break;
        }
    }
    
    va_end(ap);
    return 0;
}