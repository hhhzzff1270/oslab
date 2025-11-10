#include "trap.h"
#include "printf.h"
#include <stdint.h>

/* 使用 QEMU virt 常用 CLINT 地址映射 */
#define CLINT_BASE      0x02000000UL
#define CLINT_MTIME     (CLINT_BASE + 0xBFF8UL)
#define CLINT_MTIMECMP  (CLINT_BASE + 0x4000UL) /* hart0 */

static volatile int ticks = 0;

/* 读取 memory-mapped mtime（64-bit） */
static inline uint64_t read_mtime(void) {
    volatile uint64_t *mtime = (volatile uint64_t *)CLINT_MTIME;
    return *mtime;
}

/* 
 * 安全写入 mtimecmp（避免 64-bit 寄存器使用两次 32-bit 写带来的竞态）
 * 标准序列：先把 high 置为 0xffffffff，再写 low，最后写回 high。
 */
static void write_mtimecmp(uint64_t val) {
    volatile uint32_t *cmp = (volatile uint32_t *)CLINT_MTIMECMP;
    uint32_t low = (uint32_t)val;
    uint32_t high = (uint32_t)(val >> 32);

    cmp[1] = 0xFFFFFFFFU;
    cmp[0] = low;
    cmp[1] = high;
}

/* 设置下一次定时器中断（供初始化和中断处理时调用） */
void set_next_timer(void) {
    uint64_t now = read_mtime();
    uint64_t interval = 100000; /* 调试用的较短间隔，便于观察 */
    uint64_t next = now + interval;

    write_mtimecmp(next);
}

/* 时钟中断处理函数（由 trap 分发调用） */
void timer_interrupt_handler(struct trapframe *tf) {
    ticks++;

    /* 使用 int + %d 来匹配现有 printf 实现（避免 64-bit 与 %d 导致的栈/参数错乱） */
    if (ticks % 10 == 0) {
        printf("timer: tick %d\n", ticks);
    }

    /* 重新设置下一次中断 */
    set_next_timer();
}

/* 可选：初始化函数，若系统初始化没有在别处调用 set_next_timer，可调用此函数 */
void timer_init(void) {
    printf("timer_init: init timer\n");
    set_next_timer();
}