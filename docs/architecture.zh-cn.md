<div align="center">
<p><a href="architecture.md">English</a> | 简体中文</p>
</div>

# 原理与架构

## 两类接口

| 类别 | 含义 | 示例 |
| --- | --- | --- |
| A 类 | 运行时 libc 已导出该符号，但构建环境的头文件或链接信息没有暴露。muslext 提供包装，运行时通过 `dlsym(RTLD_NEXT)` 转交给 libc。 | `statx`、`__xpg_strerror_r`、`pthread_setaffinity_np` |
| B 类 | 运行时 libc 和构建环境都没有，需要自带实现。 | `fts(3)`、GNU `obstack`、`error.h`、`utmpx` |

## A 类：运行时委托

A 类接口的实现位于 `src/` 的包装代码中：

1. 启动时/第一次调用时用 `dlsym(RTLD_NEXT, "name")` 查找真正的 libc 实现；
2. 找不到时回退到 `dlsym(RTLD_DEFAULT, "name")`；
3. 包装函数本身不会递归调用自己；
4. 如果运行时 libc 只有 `ENOSYS` 桩，包装也只能返回失败，这一点无法由扩展库修复。

## B 类：自带实现

B 类接口由 `src/` 中的独立实现提供，不依赖 glibc 的源码或行为。实现遵循 POSIX/GNU 的公开接口语义，重点覆盖：

- `fts(3)` 目录遍历；
- GNU `obstack`；
- GNU `error.h`；
- `utmpx` 文件数据库；
- `argz`、`fstab`、`glob`、`getpass`；
- `qsort_r`、`crypt_r`、`strverscmp` 等常见缺失接口。
