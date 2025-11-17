
#include "printf.h"
#include "trap.h"
#include "proc.h"
#include "tests.h"
// test entry points
extern void proc_a(void);
extern void proc_b(void);

void main() {
    proc_init();
    // 创建两个测试进程
    create_process(test_runner,"tests");
    printf("Starting scheduler...\n");
    // 进入调度器（不会返回）
    scheduler();
}