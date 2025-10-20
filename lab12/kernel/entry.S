.section .text
.global _start

_start:
    li t0, 0x10000000   
    li t1, 'S'          
    sb t1, 0(t0)

    la sp, stack_top
    li t1, 'P'
    sb t1, 0(t0)
    
    la a0, _bss_start
    la a1, _bss_end
    jal zero_bss
    
    li t1, 'B'
    sb t1, 0(t0)
    
    jal main
    
1:  j 1b

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
    .space 4096       
stack_top: