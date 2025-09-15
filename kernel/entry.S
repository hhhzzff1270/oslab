.section .text
.global _start

_start:
    # 调试：立即输出字符'S'表示启动
    li t0, 0x10000000    # UART基地址
    li t1, 'S'           # 启动标记
    sb t1, 0(t0)
    
    # 设置栈指针
    la sp, stack_top
    
    # 调试：输出字符'P'表示栈设置完成
    li t1, 'P'
    sb t1, 0(t0)
    
    # 清零BSS段
    la a0, _bss_start
    la a1, _bss_end
    jal zero_bss
    
    # 调试：输出字符'B'表示BSS清零完成
    li t1, 'B'
    sb t1, 0(t0)
    
    # 跳转到C主函数
    jal main
    
    # 如果main返回，进入死循环
1:  j 1b

# BSS段清零函数
zero_bss:
    beq a0, a1, zero_done
    sw zero, 0(a0)
    addi a0, a0, 4
    j zero_bss
zero_done:
    ret

# 栈空间
.section .bss
.align 4
stack_bottom:
    .space 4096        # 4KB栈空间
stack_top: