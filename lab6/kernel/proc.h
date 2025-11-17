#ifndef PROC_H
#define PROC_H

#include "types.h"

#define NPROC 16
#define KSTACK 4096

// 保存上下文的结构（与 swtch.S 的保存顺序一致）
struct context {
    uint64 ra;
    uint64 sp;
    uint64 s0;
    uint64 s1;
    uint64 s2;
    uint64 s3;
    uint64 s4;
    uint64 s5;
    uint64 s6;
    uint64 s7;
    uint64 s8;
    uint64 s9;
    uint64 s10;
    uint64 s11;
};

enum procstate { UNUSED, USED, RUNNABLE, RUNNING, SLEEPING, ZOMBIE };

struct proc {
    struct context context;   // 切换所保存的寄存器
    enum procstate state;
    int pid;
    char name[16];
    void *kstack;             // 指向内核栈底部
    void (*kentry)(void);     // 进程入口（内核线程）
};

// 进程管理接口
void proc_init(void);
struct proc* allocproc(void);
int create_process(void (*entry)(void), const char *name);
void scheduler(void);
void yield(void);
struct proc* myproc(void);
void exit_process(void);
// 等待并回收任意已退出（ZOMBIE）进程；返回 pid 或 -1
int wait_process(int *status);
// 简单睡眠（以 CLINT ticks 为单位），内部会 yield 以允许调度
void sleep_ticks(uint64 ticks);

#endif
