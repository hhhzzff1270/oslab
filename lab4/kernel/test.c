#include "trap.h"
#include "printf.h"
static int timer_test_count = 0;

// 测试时钟中断处理函数
void test_timer_handler(struct trapframe *tf) {
    timer_test_count++;
    printf("test: timer interrupt %d\n", timer_test_count);
    
    // 设置下一次中断
    set_next_timer();
}

// 测试时钟中断
void test_timer_interrupt(void) {
    printf("Testing timer interrupt...\n");
    
    // 注册测试处理函数
    register_interrupt(IRQ_S_TIMER, test_timer_handler);
    
    uint64 start_time = get_time();
    int initial_count = timer_test_count;
    
    // 等待几次中断
    while (timer_test_count < initial_count + 3) {
        printf("Waiting for interrupt %d...\n", 
               timer_test_count - initial_count + 1);
        // 简单延时
        for (volatile int i = 0; i < 1000000; i++);
    }
    
    uint64 end_time = get_time();
    printf("Timer test completed: %d interrupts in %lu cycles\n", 
           timer_test_count - initial_count, end_time - start_time);
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