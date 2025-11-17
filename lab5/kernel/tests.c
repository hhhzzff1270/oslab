#include "printf.h"
#include "proc.h"
#include "types.h"
#include "tests.h"
#include <stdint.h>
#include <stddef.h> // for NULL
// Forward helpers from timer.c
extern uint64_t get_time(void);
extern void sleep_ticks(uint64 ticks);

// simple task used by creation test
void simple_task(void) {
    printf("simple_task: pid running\n");
    // 做一点工作然后退出
    for (int i = 0; i < 3; i++) {
        printf("simple_task: step %d\n", i);
        yield();
    }
    exit_process();
}

// cpu intensive task for scheduler test
void cpu_intensive_task(void) {
    // 做较多计算并偶尔 yield
    for (int i = 0; i < 50; i++) {
        // 轻量工作
        for (volatile int j = 0; j < 1000; j++);
        if ((i & 7) == 0) yield();
    }
    exit_process();
}

// Simple shared buffer using busy-wait and yield (not lock-protected but sufficient for test)
#define BUF_SIZE 8
static int buf[BUF_SIZE];
static int head = 0, tail = 0, count = 0;

void shared_buffer_init(void) {
    head = tail = count = 0;
}

void producer_task(void) {
    for (int i = 0; i < 10; i++) {
        // wait for space
        while (count == BUF_SIZE) yield();
        buf[tail] = i;
        tail = (tail + 1) % BUF_SIZE;
        count++;
        printf("producer: produced %d\n", i);
        yield();
    }
    exit_process();
}

void consumer_task(void) {
    for (int i = 0; i < 10; i++) {
        while (count == 0) yield();
        int v = buf[head];
        head = (head + 1) % BUF_SIZE;
        count--;
        printf("consumer: consumed %d\n", v);
        yield();
    }
    exit_process();
}

// Tests from the assignment
void test_process_creation(void) {
    printf("Testing process creation...\n");
    int pid = create_process(simple_task, "simple_task");
    if (pid <= 0) printf("create_process failed\n");

    // 测试进程表限制
    int pids[NPROC];
    int count_created = 0;
    for (int i = 0; i < NPROC + 5; i++) {
        int p = create_process(simple_task, "simple_task");
        if (p > 0) {
            pids[count_created++] = p;
        } else {
            break;
        }
    }
    printf("Created %d processes\n", count_created);

    // 清理测试进程
    for (int i = 0; i < count_created; i++) {
        wait_process(NULL);
    }
    // wait for the original one
    wait_process(NULL);
    printf("test_process_creation completed\n");
}

void test_scheduler(void) {
    printf("Testing scheduler...\n");
    for (int i = 0; i < 3; i++) {
        create_process(cpu_intensive_task, "cpu_task");
    }
    uint64 start_time = get_time();
    // sleep some ticks (treat argument as ticks)
    sleep_ticks(2000);
    uint64 end_time = get_time();
    printf("Scheduler test completed in %lu ticks\n", end_time - start_time);
}

void test_synchronization(void) {
    printf("Testing synchronization...\n");
    shared_buffer_init();
    create_process(producer_task, "producer");
    create_process(consumer_task, "consumer");
    // 等待两个任务完成
    wait_process(NULL);
    wait_process(NULL);
    printf("Synchronization test completed\n");
}

void test_runner(void) {
    // 顺序运行测试
    test_process_creation();
    test_scheduler();
    test_synchronization();
    printf("All tests done\n");
    exit_process();
}
