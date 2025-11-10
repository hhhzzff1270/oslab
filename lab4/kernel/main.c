#include "printf.h"
#include "trap.h"

void main() {
    printf("Kernel booting...\n");
    
    // 初始化中断系统
    trap_init();
    
    printf("trap_init done\n");
    
    // 注册时钟中断处理函数
    register_interrupt(IRQ_S_TIMER, timer_interrupt_handler);
    
    printf("interrupt registered\n");
    
    // 使能时钟中断和全局中断
    enable_interrupt(IRQ_S_TIMER);
    enable_global_interrupts();
    
    printf("interrupts enabled\n");
    
    // 设置第一次定时器中断
    printf("setting timer...\n");
    set_next_timer();
    printf("timer set successfully\n");
    
    printf("Entering main loop...\n");
    
    int loop_count = 0;
    while(1) {
        asm volatile("wfi"); // 等待中断
        
        // 简单计数，防止完全卡死
        loop_count++;
        if (loop_count % 1000000 == 0) {
            printf("main loop: %d\n", loop_count);
        }
    }
}