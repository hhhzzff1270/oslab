#include "trap.h"
#include "printf.h"
#include "riscv.h"

static interrupt_handler_t interrupt_handlers[32];

void trap_init(void) {
    // 清零中断处理函数表
    for (int i = 0; i < 32; i++) {
        interrupt_handlers[i] = 0;
    }
    
    // 设置监督/特权模式陷阱向量（保持原逻辑）
    w_stvec((uint64)kernelvec);
    
    // 委托中断给S模式（使用 1UL 避免整型溢出）
    w_mideleg((1UL << IRQ_S_TIMER));
    
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

    int is_intr = (int)((cause >> 63) & 1);
    int num = (int)(cause & (~(1UL << 63)));
    // 调试输出：打印是中断还是异常以及具体编号（避免 long/格式不匹配）
    // 注意：打印频繁会影响时序，仅供调试定位问题时使用
    printf("trap: cause=%d is_intr=%d\n", num, is_intr);

    if (is_intr) {
        // 中断处理
        if (num < 32 && interrupt_handlers[num]) {
            interrupt_handlers[num](tf);
        }
    } else {
        // 异常处理
        printf("trap: exception %d\n", num);
    }
}