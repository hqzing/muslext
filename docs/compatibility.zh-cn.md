<div align="center">
<p><a href="compatibility.md">English</a> | 简体中文</p>
</div>

# 兼容性与限制

本文档列出 muslext 提供的接口及其边界，精确声明以安装后的头文件为准。

## 支持的接口

| 领域 | 接口 |
| --- | --- |
| 目录遍历 | `fts_open`、`fts_read`、`fts_children`、`fts_set`、`fts_close` |
| 对象栈 | `obstack_*` |
| 诊断 | `error`、`error_at_line`、GNU `error.h` 全局变量 |
| 登录记录 | `getutxent`、`setutxent`、`endutxent`、`getutxid`、`getutxline`、`pututxline`、`utmpxname`、`updwtmpx` |
| Argz | `argz_create`、`argz_create_sep`、`argz_add`、`argz_add_sep`、`argz_append`、`argz_count`、`argz_insert`、`argz_stringify` |
| fstab | `setfsent`、`endfsent`、`getfsent`、`getfsfile`、`getfsspec` |
| 排序 | `qsort_r` |
| 字符串/内存 | `rawmemchr`、`strverscmp`、`strtoq`、`strtouq`、`wcslcpy`、`wcslcat`、`wcstol_l`、`wcstoul_l` |
| 错误名 | `strerrorname_np`、`strerrordesc_np` |
| 信号 | `sigblock`、`sigsetmask`、`sigabbrev_np`、`sigdescr_np` |
| 数学 | `gamma`、`clog10`、`roundeven`、`roundevenf` |
| 格式化转换 | `qecvt`、`qfcvt`、`qgcvt`、`parse_printf_format` |
| Base64 | `b64_ntop`、`b64_pton` |
| 设备号 | `gnu_dev_major`、`gnu_dev_minor`、`gnu_dev_makedev` |
| libc 标识 | `gnu_get_libc_version`、`gnu_get_libc_release` |
| 路径 | `canonicalize_file_name`、`get_current_dir_name`、`getpt`、`getwd`、`closefrom`、`scandirat`、`glob_pattern_p`、`glob` |
| 目录排序 | `versionsort` |
| 密码输入 | `getpass` |
| POSIX 线程 | `pthread_yield`、`pthread_tryjoin_np`、`pthread_timedjoin_np`、`pthread_setaffinity_np`、`pthread_getaffinity_np` |
| 凭据 | `crypt_r`、shadow 数据库委托 |
| Linux 系统调用 | mount API、`pidfd_open`、`pidfd_getfd`、`getcpu`、`ntp_adjtime`、`preadv64v2`、`pwritev64v2` |
| libc 内部 | `__libc_realloc`、`__libc_memalign`、`__asinf_finite`、`__isinff`、`__isnanf`、`__fpu_control`、`malloc_trim` |

## 最小头文件

构建系统执行编译器或 configure 探测时，建议只包含目标接口的最小头文件：

| 头文件 | 用途 |
| --- | --- |
| `<muslext/getpass.h>` | `getpass` |
| `<muslext/utmpx.h>` | utmpx 登录记录接口 |
| `<muslext/qsort_r.h>` | `qsort_r` |
| `<muslext/shadow.h>` | shadow 口令查询 |
| `<muslext/statx.h>` | `statx` |
| `<muslext/gnu.h>` | GNU 常量 |
| `<muslext/program.h>` | `program_invocation_*` |
| `<muslext/crypt.h>` | `crypt`、`crypt_r` |
| `<muslext/dirent.h>` | `versionsort`、`scandirat` |
| `<muslext/fpu_control.h>` | `__fpu_control` |
| `<muslext/glob.h>` | `glob_pattern_p` |
| `<muslext/gnu-libc-version.h>` | `gnu_get_libc_version`、`gnu_get_libc_release` |
| `<muslext/malloc.h>` | `malloc_trim`、`__libc_realloc`、`__libc_memalign` |
| `<muslext/math.h>` | `gamma`、`roundeven`、`clog10`、finite/NaN 辅助函数 |
| `<muslext/pthread.h>` | `pthread_tryjoin_np`、`pthread_timedjoin_np`、`pthread_yield` |
| `<muslext/pthread_affinity.h>` | `pthread_setaffinity_np`、`pthread_getaffinity_np`；需要 `_GNU_SOURCE` |
| `<muslext/resolv.h>` | `b64_ntop`、`b64_pton` |
| `<muslext/sched.h>` | `getcpu` |
| `<muslext/setjmp.h>` | `__sigsetjmp`；需要 POSIX/GNU feature-test 宏 |
| `<muslext/signal.h>` | `sigblock`、`sigsetmask`、`sig*_np` |
| `<muslext/stdlib.h>` | `reallocarray`、`canonicalize_file_name`、`strtoq`、`qecvt` |
| `<muslext/string.h>` | `__xpg_strerror_r`、`rawmemchr`、`strverscmp`、`strerror*_np` |
| `<muslext/sys/mount.h>` | Linux mount API（`fsopen`、`open_tree` 等） |
| `<muslext/sys/sysmacros.h>` | `gnu_dev_major`、`gnu_dev_minor`、`gnu_dev_makedev` |
| `<muslext/sys/timex.h>` | `ntp_adjtime` |
| `<muslext/sys/uio.h>` | `preadv64v2`、`pwritev64v2` |
| `<muslext/unistd.h>` | `get_current_dir_name`、`getwd`、`getpt`、`closefrom`、`close_range` |
| `<muslext/wchar.h>` | `wcslcpy`、`wcslcat`、`wcstol_l`、`wcstoul_l` |
| `<fts.h>`、`<error.h>`、`<obstack.h>`、`<argz.h>` | fts、GNU error、obstack、argz |
| `<fstab.h>`、`<printf.h>`、`<sys/tree.h>`、`<sys/pidfd.h>` | fstab、printf hooks、tree 宏、pidfd |

完整接口面使用 `<muslext/muslext.h>`。

## Feature-test 宏

属于 GNU 扩展的接口需要调用方在任何系统头文件之前定义 `_GNU_SOURCE`：

- `<muslext/pthread_affinity.h>` 需要 `_GNU_SOURCE` 才能获得 `cpu_set_t`，否则明确报错；它声明 `pthread_setaffinity_np` 和 `pthread_getaffinity_np`；
- `<muslext/setjmp.h>` 需要 `_GNU_SOURCE` 或 POSIX feature-test 宏才能获得 `sigjmp_buf`。

`<muslext/muslext.h>` 只会在对应 feature-test 宏暴露类型时包含这些 GNU 依赖头文件。

## 接口说明

`fts` 使用标准的 glibc/musl-fts ABI，普通程序和测试无需修改源码即可编译。

`pthread_cancel`/`pthread_testcancel` 不提供；它们无法在 libc 外正确实现。

`FNM_EXTMATCH` 常量已定义，但扩展匹配语义尚未实现。

## 非目标

- gettext/libintl 有意不实现；请使用外部的 gettext 实现。
- POSIX AIO、locale 数据、iconv 表和密码算法不在范围内；请使用 `libiconv`、`libxcrypt` 等。

## 行为说明

- mount API、`ntp_adjtime`、`pidfd_*`、`getcpu`、`preadv64v2`/`pwritev64v2` 依赖内核能力；内核不支持时返回 `ENOSYS`。
- `__libc_*`、`__fpu_control`、`__asinf_finite`、`__isinff`、`__isnanf` 是 best-effort ABI 兼容，不保证 libc 内部语义。
- A 类包装（shadow、`pthread_setaffinity_np`/`pthread_getaffinity_np`、`statx` 等）依赖运行时 libc 的真实实现；如果运行时本身是 `ENOSYS` 桩，包装也只能返回失败。
