#ifndef TEST_H
#define TEST_H
void test_timer_handler(struct trapframe *tf);
void test_timer_interrupt(void);
void test_exception_handling(void);
void test_interrupt_overhead(void);
unsigned long get_time(void);
#endif