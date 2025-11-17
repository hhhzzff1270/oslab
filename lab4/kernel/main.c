 //url=https://github.com/hhhzzff1270/oslab/blob/9401a3ea9a65e88acd55861cdedd13f275704160/lab4/kernel/main.c
#include "printf.h"
#include "trap.h"
#include "test.h"
void main() {
    printf("Kernel booting...\n");

    // 初始化中断系统（trap_init 会写 mtvec）
    trap_init();

    printf("trap_init done\n");

    // 注册机器模式时钟中断处理函数（Machine timer 的中断号为 7）
    register_interrupt(IRQ_M_TIMER, timer_interrupt_handler);

    printf("interrupt registered\n");

    // 使能机器模式时钟中断和全局中断
    enable_interrupt(IRQ_M_TIMER);
    enable_global_interrupts();

    printf("interrupts enabled\n");

    // 设置第一次定时器中断
    printf("setting timer...\n");
    set_next_timer();
    printf("timer set successfully\n");
    test_exception_handling();
    test_timer_interrupt();
    printf("Entering main loop...\n");

    int loop_count = 0;
    while(1) {
        asm volatile("wfi"); // 等待中断

        // 简单计数，防止完全卡死
        loop_count++;
        if (loop_count % 10 == 0) {
            printf("main loop: %d\n", loop_count);
        }
    }
}