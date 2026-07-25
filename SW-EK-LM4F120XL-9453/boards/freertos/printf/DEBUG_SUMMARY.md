# FreeRTOS printf 工程调试总结

## 现象

EK-LM4F120XL 开发板上，FreeRTOS + printf 工程编译下载后，串口只打印了一轮就停止，系统死机。

## 根因分析

通过反汇编和内存布局分析，定位到 **三个** 关键问题：

---

### 问题1：`_sbrk` 返回固定地址（堆管理完全失效）

**文件：** `src/retargetserial.c`

**原代码：**

```c
caddr_t _sbrk(int incr)
{
    (void) incr;           // 直接忽略 incr 参数
    return (caddr_t) 0x20007000;  // 每次返回同一个地址
}
```

**问题：** `_sbrk` 是 newlib 的 `malloc` 底层堆管理接口。每次调用都返回固定地址 `0x20007000`，不跟踪、不推进堆指针，导致：

1. 第一次 `printf` 时，newlib 通过 `malloc` → `_sbrk` 分配 stdout 缓冲区，打印成功
2. 后续任何 `malloc` 调用也得到 `0x20007000`，内存完全重叠
3. FreeRTOS 内部数据、任务栈等被覆盖踩踏 → 系统崩溃

**修复：**

```c
extern char __heap_start;
extern char __heap_end;

caddr_t _sbrk(int incr)
{
    static char *heap_ptr = 0;
    char *prev;

    if (heap_ptr == 0)
        heap_ptr = &__heap_start;

    prev = heap_ptr;
    if (heap_ptr + incr > &__heap_end)
    {
        errno = ENOMEM;
        return (caddr_t) -1;
    }
    heap_ptr += incr;
    return (caddr_t) prev;
}
```

同时在链接脚本 `main.ld` 中添加堆区起止符号：

```
__heap_start = .;
__heap_end = ORIGIN(SRAM) + LENGTH(SRAM);
```

---

### 问题2：newlib `malloc` 锁是空函数（线程不安全）

**文件：** `src/retargetserial.c`

**现象：** 反汇编确认 newlib 的锁函数 `__retarget_lock_acquire_recursive` 和 `__retarget_lock_release_recursive` 只是 `bx lr`（直接返回，什么也不做）：

```asm
<__retarget_lock_acquire_recursive>:
    34a4:   4770        bx  lr      ; 直接返回，没有做任何保护

<__retarget_lock_release_recursive>:
    34a8:   4770        bx  lr
```

**问题：** `printf` 内部调用 `__malloc_lock` → `__retarget_lock_acquire_recursive`，但这个锁不调用 `vTaskSuspendAll()`。在 FreeRTOS 抢占式调度下：

- UART 中断在 `printf` 执行 `malloc` 期间触发
- Timer 任务同时调用 newlib 函数
- → `malloc` 内部数据结构被并发破坏 → 系统崩溃

**修复：** 覆盖 newlib 预留的 `__malloc_lock` / `__malloc_unlock` 钩子（这些是 newlib 提供的 weak symbol，可以安全覆盖）：

```c
#include "FreeRTOS.h"
#include "task.h"

void __malloc_lock(struct _reent *reent)
{
    (void) reent;
    vTaskSuspendAll();
}

void __malloc_unlock(struct _reent *reent)
{
    (void) reent;
    xTaskResumeAll();
}
```

> **注意：** 不能直接覆盖 `__sfp_lock_acquire` / `__sfp_lock_release` 或 `__retarget_lock_*` 系列，它们在 `libc.a` 中不是 weak symbol，会导致链接时多重定义冲突。

---

### 问题3：vTask 栈太小（栈溢出）

**文件：** `src/main.c`

**原代码：**

```c
StackType_t  new_stack[128];   // 静态分配
xTaskCreateStatic(vTask, "vTask", 128, NULL, 1, new_stack, &new_taskdef);
// 或者动态：
xTaskCreate(vTask, "vTask", 128, NULL, 1, NULL);
```

128 words = 512 bytes，但 `printf` 调用链的栈消耗分析：

| 函数 | 栈帧大小 |
|------|---------|
| `_vfiprintf_r` | 276 bytes（`sub sp, #276`） |
| `iprintf` | ~32 bytes |
| `_malloc_r` | ~56 bytes |
| `__sflush_r` | ~24 bytes+ |
| FreeRTOS 上下文保存 (FPU lazy stacking) | ~68 bytes |
| **合计** | **~456+ bytes** |

512 字节几乎用满，任何中断嵌套都会导致栈溢出 → 触发 `vApplicationStackOverflowHook` → `taskDISABLE_INTERRUPTS()` → 死机。

**修复：** 栈大小从 128 → 256 words（1KB）：

```c
StackType_t  new_stack[256];
xTaskCreateStatic(vTask, "vTask", 256, NULL, 1, new_stack, &new_taskdef);
```

---

## 修改文件清单

| 文件 | 修改内容 |
|------|---------|
| `src/retargetserial.c` | 重写 `_sbrk`，正确跟踪堆指针；添加 `__malloc_lock` / `__malloc_unlock` 钩子；添加 `errno.h`、`reent.h`、`FreeRTOS.h`、`task.h` 头文件 |
| `main.ld` | 添加 `__heap_start` / `__heap_end` 链接符号 |
| `src/main.c` | vTask 栈大小从 128 → 256 words；静态栈数组同步增大 |

## 内存布局

```
SRAM: 0x20000000 - 0x20008000 (32KB)
├── .data          0x20000000 - 0x20000878  (2168 bytes)
├── .bss           0x20000878 - 0x20001b58  (4832 bytes)
│   └── ucHeap     0x200009a0 - 0x200019a0  (FreeRTOS heap_4, 4KB)
├── .ARM.exidx     0x20001b58 - 0x20001b60  (8 bytes)
├── newlib _sbrk   0x20001b60 - 0x20008000  (25.7KB, newlib malloc 堆)
└── __heap_end     0x20008000
```

FreeRTOS 使用 `heap_4` 管理自己的 4KB 堆（`ucHeap`），newlib 使用 `_sbrk` 管理 BSS 之后的剩余空间。两者互不干扰。

## 经验教训

1. **`_sbrk` 必须正确实现**：返回固定地址是最常见的嵌入式 newlib 移植错误，所有 `malloc` 分配会重叠踩踏
2. **newlib 锁不能为空**：在 RTOS 环境中，`malloc` 和 stdio 不是线程安全的，必须用 `vTaskSuspendAll` / `xTaskResumeAll` 保护 `__malloc_lock` / `__malloc_unlock`
3. **使用 `printf` 的任务栈要足够大**：newlib 的 `iprintf` / `_vfiprintf_r` 内部栈帧消耗约 300+ bytes，加上调用链和 FPU 上下文保存，任务栈至少需要 1KB
4. **`configCHECK_FOR_STACK_OVERFLOW` 设为 2**：有助于在开发阶段捕获栈溢出问题（本工程已开启）
