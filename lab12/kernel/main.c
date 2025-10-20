// main.c
#include "printf.h"
int a = 1024;
void test_printf_basic(void) {
    printf(" Basic Test \n");
    printf("Integer: %d\n", 42);
    printf("Negative: %d\n", -123);
    printf("Zero: %d\n", 0);
    printf("Hex: 0x%x\n", 0xABC);
    printf("String: %s\n", "Hello, OS!");
    printf("Char: %c%c%c\n", 'X', 'Y', 'Z');
    printf("Percent: %%\n");
}

void test_printf_edge(void) {
    printf("\n  Edge Case Test \n");
    printf("INT_MAX: %d\n", 2147483647);
    printf("INT_MIN: %d\n", -2147483648); // 测试边界
    printf("NULL string: %s\n", (char*)0);
    printf("Empty string: %s\n", "");
}

int main() {
    // 1清屏
    clear_screen();

    // 2运行测试
    printf("Kernel printf() Test Started\n\n");
    test_printf_basic();
    test_printf_edge();

    printf("\nTest Finished. System Halted.\n");

    // 3停机
    while (1) {}
    return 0;
}