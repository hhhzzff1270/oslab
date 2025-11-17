#include "proc.h"
#include "printf.h"
#include "types.h"

extern void swtch(struct context *old, struct context *nw);

static struct proc proc_table[NPROC];
static char kstacks[NPROC][KSTACK];
static int nextpid = 1;
static struct proc *current = 0;
static struct context scheduler_context;

void proc_init(void) {
    for (int i = 0; i < NPROC; i++) {
        proc_table[i].state = UNUSED;
        proc_table[i].pid = 0;
        proc_table[i].kstack = &kstacks[i][0];
    }
}

struct proc* allocproc(void) {
    for (int i = 0; i < NPROC; i++) {
        if (proc_table[i].state == UNUSED) {
            struct proc *p = &proc_table[i];
            p->state = USED;
            p->pid = nextpid++;
            p->kentry = 0;
            for (int j = 0; j < 16; j++) p->name[j] = 0;
            return p;
        }
    }
    return 0;
}

// proc_start: 当从 swtch 返回到一个新进程时，它会执行这个函数，调用真实入口并退出
static void proc_start(void) {
    struct proc *p = current;
    if (p && p->kentry) {
        p->kentry();
    }
    // 入口函数返回则退出
    exit_process();
}

int create_process(void (*entry)(void), const char *name) {
    struct proc *p = allocproc();
    if (!p) return -1;

    // 初始化内核栈和上下文
    uint64 stack_top = (uint64)p->kstack + KSTACK;

    // 清零 context
    for (int i = 0; i < sizeof(struct context)/8; i++) ((uint64*)&p->context)[i] = 0;

    // 设置返回地址为 proc_start
    p->context.ra = (uint64)proc_start;
    p->context.sp = stack_top;

    p->kentry = entry;
    if (name) {
        int i=0; for (; i<15 && name[i]; i++) p->name[i]=name[i]; p->name[i]=0;
    }
    p->state = RUNNABLE;
    return p->pid;
}

struct proc* myproc(void) {
    return current;
}

int wait_process(int *status) {
    for (;;) {
        // scan for a zombie
        for (int i = 0; i < NPROC; i++) {
            struct proc *p = &proc_table[i];
            if (p->state == ZOMBIE) {
                int pid = p->pid;
                p->state = UNUSED;
                p->pid = 0;
                p->kentry = 0;
                if (status) *status = 0;
                return pid;
            }
        }
        // 暂无退出进程，放弃 CPU 并让其他进程运行
        yield();
    }
    return -1;
}

void sleep_ticks(uint64 ticks) {
    uint64 start = get_time();
    while (get_time() - start < ticks) {
        // 让出 CPU，避免长时间忙等
        yield();
    }
}

void yield(void) {
    struct proc *p = myproc();
    if (!p) return;
    p->state = RUNNABLE;
   // printf("yield: pid=%d name=%s\n", p->pid, p->name);
    // 保存当前上下文并切换回调度器
    swtch(&p->context, &scheduler_context);
}

void exit_process(void) {
    struct proc *p = myproc();
    if (!p) {
        // no current process, just return to scheduler
        swtch(&scheduler_context, &scheduler_context);
    }
    p->state = ZOMBIE;
   // printf("process %d (%s) exit\n", p->pid, p->name);
    // 切回调度器以回收
    current = 0;
    swtch(&p->context, &scheduler_context);
    // 不会返回
    while (1) {}
}

void scheduler(void) {
    int loopcount = 0;
    for (;;) {
        if ((loopcount++ % 100) == 0) {
            /* occasional heartbeat to show scheduler is active */
            //printf("scheduler: outer loop %d\n", loopcount);
        }
        int found_runnable = 0;
        for (int i = 0; i < NPROC; i++) {
            struct proc *p = &proc_table[i];
            if (p->state == RUNNABLE) {
                found_runnable = 1;
                p->state = RUNNING;
                current = p;
                // 切换到进程上下文
            //    printf("scheduler: switch to pid=%d name=%s\n", p->pid, p->name);
                swtch(&scheduler_context, &p->context);
            //    printf("scheduler: returned from pid=%d state=%d\n", p->pid, p->state);
                // 进程返回后（yield 或 exit），继续
                if (p->state == ZOMBIE) {
                    // 回收
                    p->state = UNUSED;
                    p->pid = 0;
                    p->kentry = 0;
                }
            }
        }
        /* 如果没有可运行进程，进入低功耗等待；否则立即继续下一轮调度 */
        if (!found_runnable) {
            asm volatile("wfi");
        }
    }
}
