#include "printf.h"
#include "proc.h"

void proc_a(void) {
    for (int i = 0; i < 5; i++) {
        printf("proc_a: iteration %d\n", i);
        // 简单让出 CPU（缩短循环以便更快看到输出）
        for (volatile int j = 0; j < 10000; j++);
        yield();
    }
}

void proc_b(void) {
    for (int i = 0; i < 5; i++) {
        printf("proc_b: iteration %d\n", i);
        for (volatile int j = 0; j < 10000; j++);
        yield();
    }
}
