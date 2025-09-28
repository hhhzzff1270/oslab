#ifndef _MEMLAYOUT_H
#define _MEMLAYOUT_H
#include "types.h"
#define PGSIZE 4096           // 页大小
#define PGSHIFT 12            // 页偏移位数

// 关键内存地址定义
#define KERNBASE 0x80000000L  // 内核起始地址
#define PHYSTOP  (KERNBASE + 128 * 1024 * 1024) // 物理内存上限 128MB

// 设备地址
#define UART0 0x10000000L     // UART设备地址

// 地址转换宏
#define PGROUNDUP(sz)  (((sz) + PGSIZE - 1) & ~(PGSIZE - 1))
#define PGROUNDDOWN(a) (((a)) & ~(PGSIZE - 1))

// 页表项操作
#define PTE_V (1L << 0)       // Valid
#define PTE_R (1L << 1)       // Read
#define PTE_W (1L << 2)       // Write
#define PTE_X (1L << 3)       // Execute
#define PTE_U (1L << 4)       // User

#define PTE2PA(pte) (((pte) >> 10) << 12)
#define PA2PTE(pa)  (((pa) >> 12) << 10)

// Sv39地址提取
#define VPN_SHIFT(level) (12 + 9 * (level))
#define VPN(va, level) (((va) >> VPN_SHIFT(level)) & 0x1FF)

typedef uint64 pte_t;
typedef uint64* pagetable_t;

#endif