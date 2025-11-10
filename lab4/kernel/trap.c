#include "trap.h"
#include "printf.h"
#include "riscv.h"

static interrupt_handler_t interrupt_handlers[32];

void trap_init(void) {
    // 清零中断处理函数表
    for (int i = 0; i < 32; i++) {
        interrupt_handlers[i] = 0;
    }
    
    // 设置监管模式陷阱向量
    w_stvec((uint64)kernelvec);
    
    // 委托中断给S模式
    w_mideleg((1 << IRQ_S_TIMER));
    
    printf("trap: interrupt system initialized\n");
}

void register_interrupt(int irq, interrupt_handler_t handler) {
    if (irq >= 0 && irq < 32) {
        interrupt_handlers[irq] = handler;
    }
}

void enable_interrupt(int irq) {
    if (irq >= 0 && irq < 32) {
        uint64 sie = r_sie();
        sie |= (1UL << irq);
        w_sie(sie);
    }
}

void enable_global_interrupts(void) {
    uint64 sstatus = r_sstatus();
    sstatus |= SSTATUS_SIE;
    w_sstatus(sstatus);
}

void kerneltrap(struct trapframe *tf) {
    uint64 cause = tf->cause;
    
    if (cause & (1UL << 63)) {
        // 中断处理
        cause &= ~(1UL << 63);
        if (cause < 32 && interrupt_handlers[cause]) {
            interrupt_handlers[cause](tf);
        }
    } else {
        // 异常处理
        printf("trap: exception %d\n", cause);
    }
}