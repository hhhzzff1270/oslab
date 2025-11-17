.section .text
.globl kernelvec
.align 4

# 内核陷阱向量 - 处理内核态中断和异常
kernelvec:
    # 保存所有通用寄存器到栈中，并为 trapframe 中的 epc/status/cause/tval/insn 留出空间
    # 31 个寄存器 (ra..t6) * 8 = 248, + 5 个 CSR 字段 *8 = 40 => 288 字节
    addi sp, sp, -288

    # 保存通用寄存器（不要保存或恢复 sp 本身）
    sd ra, 0(sp)
    sd gp, 16(sp)
    sd tp, 24(sp)
    sd t0, 32(sp)
    sd t1, 40(sp)
    sd t2, 48(sp)
    sd s0, 56(sp)
    sd s1, 64(sp)
    sd a0, 72(sp)
    sd a1, 80(sp)
    sd a2, 88(sp)
    sd a3, 96(sp)
    sd a4, 104(sp)
    sd a5, 112(sp)
    sd a6, 120(sp)
    sd a7, 128(sp)
    sd s2, 136(sp)
    sd s3, 144(sp)
    sd s4, 152(sp)
    sd s5, 160(sp)
    sd s6, 168(sp)
    sd s7, 176(sp)
    sd s8, 184(sp)
    sd s9, 192(sp)
    sd s10, 200(sp)
    sd s11, 208(sp)
    sd t3, 216(sp)
    sd t4, 224(sp)
    sd t5, 232(sp)
    sd t6, 240(sp)

    # 保存 CSR 到 trapframe 区域（相对于 new sp 的偏移）
    # epc -> 248(sp)
    csrr t0, mepc
    sd t0, 248(sp)
    # status -> 256(sp)
    csrr t0, mstatus
    sd t0, 256(sp)
    # cause -> 264(sp)
    csrr t0, mcause
    sd t0, 264(sp)
    # tval -> 272(sp)
    csrr t0, mtval
    sd t0, 272(sp)
    # insn（这里暂置为0，C 代码可自行填充） -> 280(sp)
    li t0, 0
    sd t0, 280(sp)

    # 调用 C 语言的 trap 处理（参数：sp）
    mv a0, sp
    call kerneltrap

    # kerneltrap 可能修改了 tf->epc，把 epc 写回 mepc（以便 mret 返回到正确位置）
    ld t0, 248(sp)
    csrw mepc, t0

    # 恢复通用寄存器（不要恢复 sp，这样后续基于 sp 的 ld 能正确访问保存的内容）
    ld ra, 0(sp)
    ld gp, 16(sp)
    ld tp, 24(sp)
    ld t0, 32(sp)
    ld t1, 40(sp)
    ld t2, 48(sp)
    ld s0, 56(sp)
    ld s1, 64(sp)
    ld a0, 72(sp)
    ld a1, 80(sp)
    ld a2, 88(sp)
    ld a3, 96(sp)
    ld a4, 104(sp)
    ld a5, 112(sp)
    ld a6, 120(sp)
    ld a7, 128(sp)
    ld s2, 136(sp)
    ld s3, 144(sp)
    ld s4, 152(sp)
    ld s5, 160(sp)
    ld s6, 168(sp)
    ld s7, 176(sp)
    ld s8, 184(sp)
    ld s9, 192(sp)
    ld s10, 200(sp)
    ld s11, 208(sp)
    ld t3, 216(sp)
    ld t4, 224(sp)
    ld t5, 232(sp)
    ld t6, 240(sp)

    addi sp, sp, 288

    # 返回到产生陷阱的特权级（现在我们处理的是 Machine trap，所以用 mret）
    mret

# 机器模式定时器中断向量（可保留或不使用）
.globl timervec
.align 4