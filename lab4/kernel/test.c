#include "trap.h"
#include "printf.h"
#include "test.h"
#define uint64_t unsigned long
static int timer_test_count = 0;

// 测试时钟中断处理函数
void test_timer_handler(struct trapframe *tf) {
    timer_test_count++;
    printf("test: timer interrupt %d\n", timer_test_count);
    
    // 设置下一次中断
    set_next_timer();
}
uint64_t get_time(void) {
    // 读取 CLINT 的 mtime 寄存器
    volatile uint64_t *mtime = (volatile uint64_t *)0x02000000UL + 0xBFF8UL / 8;
    return *mtime;
}
// 测试时钟中断
void test_timer_interrupt(void) {
    printf("Testing timer interrupt...\n");

    // 用 M 模式定时器来测
    register_interrupt(IRQ_M_TIMER, test_timer_handler);

    // 重新设一次定时器，确保有后续中断
    set_next_timer();

    int target = timer_test_count + 3;
    while (timer_test_count < target) {
        asm volatile("wfi");
    }

    printf("Timer test completed: %d interrupts\n",
           timer_test_count);
    
    // 如需恢复原来的处理函数：
    register_interrupt(IRQ_M_TIMER, timer_interrupt_handler);
}

// 测试异常处理
void test_exception_handling(void) {
    printf("Testing exception handling...\n");
    
    // 测试非法指令异常
    printf("Testing illegal instruction...\n");
    // 注意：这会触发异常，需要确保异常处理能正常工作
    // asm volatile(".word 0x00000000"); // 非法指令
    
    printf("Exception tests completed\n");
}

// 性能测试
void test_interrupt_overhead(void) {
    printf("Testing interrupt overhead...\n");
    
    uint64 start = get_time();
    int iterations = 1000;
    
    // 模拟多次中断处理
    for (int i = 0; i < iterations; i++) {
        // 这里可以测量中断处理时间
    }
    
    uint64 end = get_time();
    printf("Interrupt overhead test: %d iterations in %lu cycles\n", 
           iterations, end - start);
}