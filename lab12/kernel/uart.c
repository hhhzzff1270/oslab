#define UART_BASE 0x10000000

#define UART_THR (*(volatile unsigned char *)(UART_BASE + 0x00)) // 发送保持寄存器
#define UART_LSR (*(volatile unsigned char *)(UART_BASE + 0x05)) // 线路状态寄存器

static int uart_tx_ready(void) {
    return (UART_LSR & 0x20); // 第5位为1表示发送器空
}

void uart_putc(char c) {
    while (!uart_tx_ready()) {
    }
    // 发送字符
    UART_THR = c;
}

// 输出字符串
void uart_puts(const char* s) {
    while (*s) {
        uart_putc(*s);
        s++;
    }
}

void uart_init(void) {
}