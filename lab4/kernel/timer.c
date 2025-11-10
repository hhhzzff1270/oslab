#include "trap.h"
#include "printf.h"
#include <stdint.h>

/* 使用 QEMU virt 常用 CLINT 地址映射 */
#define CLINT_BASE      0x02000000UL
#define CLINT_MTIME     (CLINT_BASE + 0xBFF8UL)
#define CLINT_MTIMECMP_BASE  (CLINT_BASE + 0x4000UL) /* base for hart0 */

/* 小心不要用 64-bit printf 格式，打印时用 hartid 和低 32 位进行调试 */
static volatile int ticks = 0;

static inline uint64_t read_mtime(void) {
    volatile uint64_t *mtime = (volatile uint64_t *)CLINT_MTIME;
    return *mtime;
}

/* 安全写入 mtimecmp 到指定 hart 的地址（使用 32-bit 顺序写以避免竞态） */
static void write_mtimecmp_hart(uint64_t val, uint64_t hartid) {
    volatile uint32_t *cmp = (volatile uint32_t *)(CLINT_MTIMECMP_BASE + hartid * 8);
    uint32_t low = (uint32_t)val;
    uint32_t high = (uint32_t)(val >> 32);

    /* 标准序列：先把 high 置为 0xffffffff，写 low，再写 high */
    cmp[1] = 0xFFFFFFFFU;
    cmp[0] = low;
    cmp[1] = high;
}

/* 设置下一次定时器中断（供初始化和中断处理时调用） */
void set_next_timer(void) {
    uint64_t now = read_mtime();
    uint64_t interval = 100000; /* 调试用间隔 */
    uint64_t next = now + interval;

    /* 读取 mhartid，确保写到正确 hart 的 mtimecmp */
    uint64_t hartid;
    asm volatile("csrr %0, mhartid" : "=r"(hartid));

    /* 调试打印：打印 hartid 与时间的低 32 位，避免 64-bit 格式问题 */
    printf("set_next_timer: hartid=%d now_lo=0x%x next_lo=0x%x\n",
           (int)hartid, (uint32_t)now, (uint32_t)next);

    write_mtimecmp_hart(next, hartid);

    /* 读回验证（读回低 32 位） */
    volatile uint64_t *cmp64 = (volatile uint64_t *)(CLINT_MTIMECMP_BASE + hartid * 8);
    uint64_t readback = *cmp64;
    printf("set_next_timer: wrote mtimecmp_lo=0x%x readback_lo=0x%x\n",
           (uint32_t)next, (uint32_t)readback);
}

/* 时钟中断处理函数（由 trap 分发调用） */
void timer_interrupt_handler(struct trapframe *tf) {
    ticks++;

    if (ticks % 10 == 0) {
        printf("timer: tick %d\n", ticks);
    }

    /* 重新设置下一次中断 */
    set_next_timer();
}

/* 可被外部调用的初始化（可选） */
void timer_init(void) {
    printf("timer_init: init timer\n");
    set_next_timer();
}