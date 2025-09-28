#ifndef _VMM_H
#define _VMM_H

#include "types.h"
#include "memlayout.h"
static pagetable_t kernel_pagetable = 0;
pagetable_t create_pagetable(void);
void destroy_pagetable(pagetable_t pt);
int map_page(pagetable_t pt, uint64 va, uint64 pa, int perm);
int unmap_page(pagetable_t pt, uint64 va);
pte_t* walk(pagetable_t pt, uint64 va, int alloc);
void dump_pagetable(pagetable_t pt, int level);

// 内核页表函数
void kvminit(void);
void kvminithart(void);
pagetable_t get_kernel_pagetable(void);

#endif