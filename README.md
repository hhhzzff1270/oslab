
一个在QEMU中运行的最小RISC-V操作系统，输出"Hello OS"。

## 功能特性

- ✅ RISC-V 64位架构支持
- ✅ 最小启动流程（汇编引导）
- ✅ UART串口输出
- ✅ BSS段自动清零
- ✅ 栈空间设置
- ✅ 从汇编跳转到C代码

## 项目结构
hello-os/
├── kernel/
│ ├── entry.S # 启动汇编代码（设置栈、清零BSS）
│ ├── kernel.ld # 链接脚本（内存布局）
│ ├── uart.c # 串口驱动（字符输出）
│ └── main.c # 主函数（Hello OS输出）
├── Makefile # 构建系统
├── README.md # 项目说明
└── .gitignore # Git忽略规则