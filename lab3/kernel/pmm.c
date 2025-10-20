#include "pmm.h"
#include "memlayout.h"
#include "printf.h"
#include <string.h>
extern char end[]; // 定义在链接脚本中，内核结束地址

struct run {
    struct run* next;
};
static struct run* free_list = 0;
static uint64 free_page_count = 0;
static uint64 total_page_count = 0;

void pmm_init(void) {
    // 计算可分配内存范围：从end开始到PHYSTOP
    uint64 start = PGROUNDUP((uint64)end);
    uint64 end_addr = PHYSTOP;
    
    printf("PMM: initializing physical memory manager\n");
    printf("PMM: free memory range [%p - %p]\n", start, end_addr);
    
    // 将所有可用页添加到空闲链表
    for (uint64 pa = start; pa < end_addr; pa += PGSIZE) {
        free_page((void*)pa);
        total_page_count++;
    }
    
    free_page_count = total_page_count;
    printf("PMM: initialized with %d free pages (%d KB total)\n", 
           free_page_count, (total_page_count * PGSIZE) / 1024);
}

void* alloc_page(void) {
    if (!free_list) {
        printf("PMM: out of memory!\n");
        return 0;
    }
    
    struct run* r = free_list;
    free_list = r->next;
    free_page_count--;
    
    // 清空页面内容，防止信息泄漏
    memset((void*)r, 0, PGSIZE);
    
    return (void*)r;
}

void free_page(void* page) {
    if ((uint64)page % PGSIZE != 0) {
        printf("PMM: free_page called with unaligned address %p\n", page);
        return;
    }
    
    struct run* r = (struct run*)page;
    r->next = free_list;
    free_list = r;
    free_page_count++;
}

uint64 get_free_page_count(void) {
    return free_page_count;
}