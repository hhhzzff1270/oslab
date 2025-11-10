#include "printf.h"
#include "pmm.h"
#include "vmm.h"

void test_physical_memory(void) {
    printf("=== Testing Physical Memory Manager ===\n");
    
    void* page1 = alloc_page();
    void* page2 = alloc_page();
    printf("Allocated pages: %p, %p\n", page1, page2);
    
    // 测试数据写入
    *(uint64*)page1 = 0x1234567890ABCDEF;
    printf("Write test: 0x%lx\n", *(uint64*)page1);
    printf("Before Free pages: %d\n", get_free_page_count());
    free_page(page1);
    free_page(page2);
    printf("Free pages: %d\n", get_free_page_count());
}

void test_virtual_memory(void) {
    printf("\n=== Testing Virtual Memory Manager ===\n");
    
    pagetable_t pt = create_pagetable();
    printf("Created page table at %p\n", pt);
    
    // 分配物理页并建立映射
    void* phys_page = alloc_page();
    uint64 virt_addr = 0x1000000; // 任意虚拟地址
    printf("%d" , VPN(virt_addr, 1));
    if (map_page(pt, virt_addr, (uint64)phys_page, PTE_R | PTE_W) == 0) {
        printf("Mapped VA %p -> PA %p\n", virt_addr, phys_page);
        
        // 通过虚拟地址访问（需要激活页表后才能实际测试）
        // *(uint64*)virt_addr = 0xCAFEBABE;
        // printf("Value at VA: 0x%lx\n", *(uint64*)virt_addr);
    }
    
    dump_pagetable(pt, 0);
    destroy_pagetable(pt);
    free_page(phys_page);
}

void test_kernel_virtual_memory(void) {
    printf("\n=== Testing Kernel Virtual Memory ===\n");
    
    kvminit();
    printf("Kernel page table ready\n");
    
    // 测试内核地址访问（在启用分页前）
    printf("Kernel code at: %p\n", test_kernel_virtual_memory);
    printf("Kernel data value: %p\n", &kernel_pagetable);
    
    kvminithart();
    printf("Virtual memory enabled - system should continue working\n");
}

int main() {
    printf("Memory Management System Test\n");
    printf("=============================\n");
    
    pmm_init();
    test_physical_memory();
    test_virtual_memory();
    test_kernel_virtual_memory();
    
    printf("\nAll tests completed successfully!\n");
    printf("System is running in idle loop...\n");
    
    while (1) {}
    return 0;
}