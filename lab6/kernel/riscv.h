#ifndef _RISCV_H_
#define _RISCV_H_

#include "types.h"

// CSR寄存器
#define SSTATUS 0x100
#define SIE     0x104
#define STVEC   0x105
#define SIP     0x144

// 状态寄存器位
#define SSTATUS_SIE (1L << 1)

// 正确声明汇编函数 - 使用 extern "C" 风格
void kernelvec(void);
void timervec(void);

// CSR读写函数
static inline uint64 r_sstatus(void) {
    uint64 x;
    asm volatile("csrr %0, sstatus" : "=r" (x));
    return x;
}

static inline void w_sstatus(uint64 x) {
    asm volatile("csrw sstatus, %0" : : "r" (x));
}

static inline void w_stvec(uint64 x) {
    asm volatile("csrw stvec, %0" : : "r" (x));
}

static inline uint64 r_sie(void) {
    uint64 x;
    asm volatile("csrr %0, sie" : "=r" (x));
    return x;
}

static inline void w_sie(uint64 x) {
    asm volatile("csrw sie, %0" : : "r" (x));
}

static inline void w_mideleg(uint64 x) {
    asm volatile("csrw mideleg, %0" : : "r" (x));
}

static inline void w_medeleg(uint64 x) {
    asm volatile("csrw medeleg, %0" : : "r" (x));
}

#endif