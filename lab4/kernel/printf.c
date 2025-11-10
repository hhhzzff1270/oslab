
#include <stdarg.h>
#include "printf.h"
#include "types.h"
extern void console_putc(char c);
// 添加memset函数实现
void* memset(void* dest, int val, size_t len) {
    unsigned char* ptr = dest;
    while (len-- > 0)
        *ptr++ = val;
    return dest;
}
static void print_number(uint64 num, int base, int sign, int is_long) {
    char digits[] = "0123456789abcdef";
    char buf[64]; 
    char *ptr = &buf[63];
    *ptr = '\0';
    uint64 n;

    if (sign) {
        if (is_long && (int64)num < 0) {
            n = -(int64)num;
            console_putc('-');
        } else if (!is_long && (int32)num < 0) {
            n = -(int32)num;
            console_putc('-');
        } else {
            n = num;
        }
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
    int long_flag = 0;
    
    va_start(ap, fmt);

    for (int i = 0; fmt[i]; i++) {
        char c = fmt[i];
        
        if (state == 0) {
            if (c == '%') {
                state = 1;
                long_flag = 0;
            } else {
                console_putc(c);
            }
        } else if (state == 1) {
            if (c == 'l') {
                long_flag = 1;
                continue;  // 继续读取下一个格式字符
            }
            
            switch (c) {
                case 'd':  // 整数
                    if (long_flag) {
                        print_number(va_arg(ap, long), 10, 1, 1);
                    } else {
                        print_number(va_arg(ap, int), 10, 1, 0);
                    }
                    break;
                    
                case 'u':  // 无符号整数
                    if (long_flag) {
                        print_number(va_arg(ap, unsigned long), 10, 0, 1);
                    } else {
                        print_number(va_arg(ap, unsigned int), 10, 0, 0);
                    }
                    break;
                    
                case 'x':  // 十六进制
                    if (long_flag) {
                        print_number(va_arg(ap, unsigned long), 16, 0, 1);
                    } else {
                        print_number(va_arg(ap, unsigned int), 16, 0, 0);
                    }
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
                    
                case 'p':  // 指针格式 (可选添加)
                    console_puts("0x");
                    print_number((uint64)va_arg(ap, void*), 16, 0, 1);
                    break;
                    
                default:
                    // 输出未知格式
                    console_putc('%');
                    if (long_flag) console_putc('l');
                    console_putc(c);
                    break;
            }
            state = 0;
            long_flag = 0;
        }
    }
    va_end(ap);
    return 0; 
}