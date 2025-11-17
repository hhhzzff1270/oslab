#ifndef _TRAP_H_
#define _TRAP_H_

#include "types.h"

// 中断处理函数类型
typedef void (*interrupt_handler_t)(struct trapframe *tf);

// 陷阱帧结构
struct trapframe {
    uint64 ra;
    uint64 sp;
    uint64 gp;
    uint64 tp;
    uint64 t0;
    uint64 t1;
    uint64 t2;
    uint64 s0;
    uint64 s1;
    uint64 a0;
    uint64 a1;
    uint64 a2;
    uint64 a3;
    uint64 a4;
    uint64 a5;
    uint64 a6;
    uint64 a7;
    uint64 s2;
    uint64 s3;
    uint64 s4;
    uint64 s5;
    uint64 s6;
    uint64 s7;
    uint64 s8;
    uint64 s9;
    uint64 s10;
    uint64 s11;
    uint64 t3;
    uint64 t4;
    uint64 t5;
    uint64 t6;

    uint64 epc;
    uint64 status;
    uint64 cause;
    uint64 tval;
    uint64 insn;
};

// 中断类型：保留 S-mode 编号，新增 Machine timer 编号
#define IRQ_S_TIMER  5
#define IRQ_M_TIMER  7

// 函数声明
void trap_init(void);
void register_interrupt(int irq, interrupt_handler_t handler);
void enable_interrupt(int irq);
void enable_global_interrupts(void);
void set_next_timer(void);
void kerneltrap(struct trapframe *tf);

// 添加定时器中断处理函数声明
void timer_interrupt_handler(struct trapframe *tf);

#endif