#define UART_BASE 0x10000000

// UART寄存器定义
#define UART_THR (*(volatile unsigned char *)(UART_BASE + 0x00)) // 发送保持寄存器
#define UART_LSR (*(volatile unsigned char *)(UART_BASE + 0x05)) // 线路状态寄存器

// 检查发送器是否就绪（THRE位）
static int uart_tx_ready(void) {
    return (UART_LSR & 0x20); // 第5位为1表示发送器空
}

// 输出单个字符
void uart_putc(char c) {
    // 等待发送器就绪
    while (!uart_tx_ready()) {
        // 空循环等待
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

// 简单的UART初始化（最小实现）
void uart_init(void) {
    // 对于QEMU virt，UART通常已经初始化好了
    // 这里可以留空或添加必要的初始化代码
}