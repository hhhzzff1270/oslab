#include "trap.h"
#include "printf.h"
#include "riscv.h"

static interrupt_handler_t interrupt_handlers[32];

void trap_init(void) {
    // 清零中断处理函数表
    for (int i = 0; i < 32; i++) {
        interrupt_handlers[i] = 0;
    }

    // 设置 Machine 模式陷阱向量 (因为 entry.S 在 M-mode 下直接进入 main)
    extern void kernelvec(void);
    asm volatile("csrw mtvec, %0" :: "r"(kernelvec));

    // 不委派定时器中断给 S-mode：不要写 mideleg（保持默认或按需要设置）
    // 如果代码里有 w_mideleg 的调用，请移除或注释

    printf("trap: interrupt system initialized\n");
}

void register_interrupt(int irq, interrupt_handler_t handler) {
    if (irq >= 0 && irq < 32) {
        interrupt_handlers[irq] = handler;
    }
}

void enable_interrupt(int irq) {
    if (irq >= 0 && irq < 32) {
        uint64 mie;
        asm volatile("csrr %0, mie" : "=r"(mie));
        mie |= (1UL << irq);
        asm volatile("csrw mie, %0" :: "r"(mie));
    }
}

void enable_global_interrupts(void) {
    uint64 mstatus;
    asm volatile("csrr %0, mstatus" : "=r"(mstatus));
    // MIE 在 mstatus 中的位，设置 MIE 以允许机器级中断
    mstatus |= (1UL << 3);
    asm volatile("csrw mstatus, %0" :: "r"(mstatus));
}

void kerneltrap(struct trapframe *tf) {
    uint64 cause = tf->cause;

    int is_intr = (int)((cause >> 63) & 1);
    int num = (int)(cause & (~(1UL << 63)));
    // 调试输出：打印是中断还是异常以及具体编号
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