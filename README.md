# Hello OS

一个最小的RISC-V操作系统，在QEMU中输出"Hello OS"。

## 功能特性

- RISC-V 64位架构支持
- 最小启动流程
- UART串口输出
- 简单的内存管理

## 构建和运行

### 环境要求
- riscv64-unknown-elf-gcc
- qemu-system-riscv64
- make

### 编译运行
```bash
make
make run