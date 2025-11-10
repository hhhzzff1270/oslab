#include "trap.h"
#include "printf.h"

static volatile uint64 ticks = 0;

// 时钟中断处理函数
void timer_interrupt_handler(struct trapframe *tf) {
    ticks++;
    
    // 每10个ticks打印一次（调试用）
    if (ticks % 10 == 0) {
        printf("timer: tick %d\n", ticks);
    }
    
    // 设置下一次定时器中断
    set_next_timer();
}

// 使用CSR直接读取时间
uint64 get_time(void) {
    uint64 time;
    asm volatile("csrr %0, time" : "=r"(time));
    return time;
}

// 设置下一次定时器中断（简化版）
void set_next_timer(void) {
    uint64 time = get_time();
    uint64 next = time + 1000000; // 较小的间隔，便于测试
    
    // 使用内联汇编直接设置定时器，避免SBI调用问题
    asm volatile(
        "mv a0, %0\n"
        "li a7, 0x54494D45\n"
        "li a6, 0\n"
        "ecall"
        : 
        : "r"(next)
        : "a0", "a6", "a7", "memory"
    );
}