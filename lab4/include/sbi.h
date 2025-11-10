#ifndef _SBI_H_
#define _SBI_H_

#include "types.h"

// SBI扩展ID
#define SBI_EXT_TIME 0x54494D45

// SBI函数ID
#define SBI_SET_TIMER 0

// SBI调用声明
uint64 sbi_call(uint64 arg0, uint64 arg1, uint64 arg2, 
                uint64 arg3, uint64 arg4, uint64 arg5,
                uint64 fid, uint64 eid);
uint64 get_time(void);
void sbi_set_timer(uint64 time);
void set_next_timer(void);
void timer_interrupt_handler(struct trapframe *tf);
#endif