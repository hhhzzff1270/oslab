#include "vmm.h"
#include "pmm.h"
#include "printf.h"
#include "string.h"


// 创建新页表
pagetable_t create_pagetable(void) {
    pagetable_t pt = (pagetable_t)alloc_page();
    if (pt) {
        memset(pt, 0, PGSIZE); // 清空页表
    }
    return pt;
}

// 销毁页表及其所有中间页表
void destroy_pagetable(pagetable_t pt) {
    // 递归释放所有页表页
    for (int i = 0; i < 512; i++) {
        if (pt[i] & PTE_V) {
            if ((pt[i] & (PTE_R | PTE_W | PTE_X)) == 0) {
                // 中间页表，递归释放
                destroy_pagetable((pagetable_t)PTE2PA(pt[i]));
            }
            free_page((void*)PTE2PA(pt[i]));
        }
    }
    free_page((void*)pt);
}

// 页表遍历函数
pte_t* walk(pagetable_t pt, uint64 va, int alloc) {
    for (int level = 2; level > 0; level--) {
        pte_t* pte = &pt[VPN(va, level)];
        if (*pte & PTE_V) {
            pt = (pagetable_t)PTE2PA(*pte);
        } else {
            if (!alloc || (pt = (pagetable_t)alloc_page()) == 0)
                return 0;
            memset(pt, 0, PGSIZE);
            *pte = PA2PTE((uint64)pt) | PTE_V;
        }
    }
    return &pt[VPN(va, 0)];
}

// 建立页映射
int map_page(pagetable_t pt, uint64 va, uint64 pa, int perm) {
    if (va % PGSIZE != 0 || pa % PGSIZE != 0) {
        printf("VMM: unaligned va=%p or pa=%p\n", va, pa);
        return -1;
    }
    
    pte_t* pte = walk(pt, va, 1);
    if (!pte) {
        printf("VMM: walk failed for va=%p\n", va);
        return -1;
    }
    
    if (*pte & PTE_V) {
        printf("VMM: va=%p already mapped\n", va);
        return -1;
    }
    
    *pte = PA2PTE(pa) | perm | PTE_V;
    return 0;
}

// 打印页表内容（调试用）
void dump_pagetable(pagetable_t pt, int level) {
    for (int i = 0; i < 512; i++) {
        if (pt[i] & PTE_V) {
            for (int j = 0; j < level; j++) printf("  ");
            printf("PTE[%d]: 0x%p -> 0x%p, perm: ", 
                   i, pt[i], PTE2PA(pt[i]));
            
            if (pt[i] & PTE_R) printf("R");
            if (pt[i] & PTE_W) printf("W");
            if (pt[i] & PTE_X) printf("X");
            if (pt[i] & PTE_U) printf("U");
            printf("\n");
            
            if ((pt[i] & (PTE_R | PTE_W | PTE_X)) == 0) {
                dump_pagetable((pagetable_t)PTE2PA(pt[i]), level + 1);
            }
        }
    }
}

// 初始化内核页表
void kvminit(void) {
    kernel_pagetable = create_pagetable();
    
    // 恒等映射内核代码和数据
    map_page(kernel_pagetable, KERNBASE, KERNBASE, PTE_R | PTE_W | PTE_X);
    
    // 映射UART设备
    map_page(kernel_pagetable, UART0, UART0, PTE_R | PTE_W);
    
    printf("VMM: kernel page table created\n");
}

// 激活内核页表
void kvminithart(void) {
    uint64 satp_value = ((uint64)8 << 60) | ((uint64)kernel_pagetable >> 12);
    asm volatile("csrw satp, %0" : : "r"(satp_value));
    asm volatile("sfence.vma");
    printf("VMM: satp register set, virtual memory enabled\n");
}

pagetable_t get_kernel_pagetable(void) {
    return kernel_pagetable;
}