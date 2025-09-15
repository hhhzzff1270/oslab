#include "printf.h"

// 测试函数
void test_printf_basic(void) {
    printf("Testing integer: %d\n", 42);
    printf("Testing negative: %d\n", -123);
    printf("Testing zero: %d\n", 0);
    printf("Testing hex: %x\n", 0xABC);
    printf("Testing string: %s\n", "Hello");
    printf("Testing char: %c\n", 'X');
    printf("Testing percent: %%\n");
}

void test_printf_edge_cases(void) {
    printf("INT_MAX: %d\n", 2147483647);
    printf("INT_MIN: %d\n", -2147483648);
    printf("NULL string: %s\n", (char*)0);
    printf("Empty string: %s\n", "");
}

int main() {
    // 清屏
    clear_screen();
    
    printf("===================================\n");
    printf("    Hello OS - printf Test\n");
    printf("===================================\n\n");
    
    // 基本测试
    printf("=== Basic Tests ===\n");
    test_printf_basic();
    printf("\n");
    
    // 边界测试
    printf("=== Edge Cases ===\n");
    test_printf_edge_cases();
    printf("\n");
    
    // 格式化组合测试
    printf("=== Format Combinations ===\n");
    printf("Number: %d, Hex: %x, String: %s\n", 255, 255, "test");
    printf("Char: %c, Percent: %%, Number: %d\n", 'A', 100);
    
    printf("\n=== Test Complete ===\n");
    printf("System is running in idle loop...\n");
    
    while (1) {}
    return 0;
}