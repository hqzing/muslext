<div align="center">
<p>English | <a href="compatibility.zh-cn.md">简体中文</a></p>
</div>

# Compatibility and limitations

This document lists the interfaces muslext provides and the boundaries of
that support.  See the installed headers for exact prototypes.

## Supported interfaces

| Area | Interfaces |
| --- | --- |
| Directory traversal | `fts_open`, `fts_read`, `fts_children`, `fts_set`, `fts_close` |
| Object stacks | `obstack_*` |
| Diagnostics | `error`, `error_at_line`, GNU `error.h` globals |
| Login records | `getutxent`, `setutxent`, `endutxent`, `getutxid`, `getutxline`, `pututxline`, `utmpxname`, `updwtmpx` |
| Argz | `argz_create`, `argz_create_sep`, `argz_add`, `argz_add_sep`, `argz_append`, `argz_count`, `argz_insert`, `argz_stringify` |
| fstab | `setfsent`, `endfsent`, `getfsent`, `getfsfile`, `getfsspec` |
| Sorting | `qsort_r` |
| String/memory | `rawmemchr`, `strverscmp`, `strtoq`, `strtouq`, `wcslcpy`, `wcslcat`, `wcstol_l`, `wcstoul_l` |
| Error names | `strerrorname_np`, `strerrordesc_np` |
| Signals | `sigblock`, `sigsetmask`, `sigabbrev_np`, `sigdescr_np` |
| Math | `gamma`, `clog10`, `roundeven`, `roundevenf` |
| Formatted conversion | `qecvt`, `qfcvt`, `qgcvt`, `parse_printf_format` |
| Base64 | `b64_ntop`, `b64_pton` |
| Devices | `gnu_dev_major`, `gnu_dev_minor`, `gnu_dev_makedev` |
| Libc identity | `gnu_get_libc_version`, `gnu_get_libc_release` |
| Paths | `canonicalize_file_name`, `get_current_dir_name`, `getpt`, `getwd`, `closefrom`, `scandirat`, `glob_pattern_p`, `glob` |
| Directory sorting | `versionsort` |
| Password input | `getpass` |
| POSIX threads | `pthread_yield`, `pthread_tryjoin_np`, `pthread_timedjoin_np`, `pthread_setaffinity_np`, `pthread_getaffinity_np` |
| Credentials | `crypt_r`, shadow database delegation |
| Linux syscalls | mount API, `pidfd_open`, `pidfd_getfd`, `getcpu`, `ntp_adjtime`, `preadv64v2`, `pwritev64v2` |
| libc internals | `__libc_realloc`, `__libc_memalign`, `__asinf_finite`, `__isinff`, `__isnanf`, `__fpu_control`, `malloc_trim` |

## Minimal headers

When a build system runs compiler or configure probes, include only the minimal header for the target interface:

| Header | Purpose |
| --- | --- |
| `<muslext/getpass.h>` | `getpass` |
| `<muslext/utmpx.h>` | utmpx login records |
| `<muslext/qsort_r.h>` | `qsort_r` |
| `<muslext/shadow.h>` | shadow password queries |
| `<muslext/statx.h>` | `statx` |
| `<muslext/gnu.h>` | GNU constants |
| `<muslext/program.h>` | `program_invocation_*` |
| `<muslext/crypt.h>` | `crypt`, `crypt_r` |
| `<muslext/dirent.h>` | `versionsort`, `scandirat` |
| `<muslext/fpu_control.h>` | `__fpu_control` |
| `<muslext/glob.h>` | `glob_pattern_p` |
| `<muslext/gnu-libc-version.h>` | `gnu_get_libc_version`, `gnu_get_libc_release` |
| `<muslext/malloc.h>` | `malloc_trim`, `__libc_realloc`, `__libc_memalign` |
| `<muslext/math.h>` | `gamma`, `roundeven`, `clog10`, finite/NaN helpers |
| `<muslext/pthread.h>` | `pthread_tryjoin_np`, `pthread_timedjoin_np`, `pthread_yield` |
| `<muslext/pthread_affinity.h>` | `pthread_setaffinity_np`, `pthread_getaffinity_np`; requires `_GNU_SOURCE` |
| `<muslext/resolv.h>` | `b64_ntop`, `b64_pton` |
| `<muslext/sched.h>` | `getcpu` |
| `<muslext/setjmp.h>` | `__sigsetjmp`; requires POSIX/GNU feature macros |
| `<muslext/signal.h>` | `sigblock`, `sigsetmask`, `sig*_np` |
| `<muslext/stdlib.h>` | `reallocarray`, `canonicalize_file_name`, `strtoq`, `qecvt` |
| `<muslext/string.h>` | `__xpg_strerror_r`, `rawmemchr`, `strverscmp`, `strerror*_np` |
| `<muslext/sys/mount.h>` | Linux mount API (`fsopen`, `open_tree`, ...) |
| `<muslext/sys/sysmacros.h>` | `gnu_dev_major`, `gnu_dev_minor`, `gnu_dev_makedev` |
| `<muslext/sys/timex.h>` | `ntp_adjtime` |
| `<muslext/sys/uio.h>` | `preadv64v2`, `pwritev64v2` |
| `<muslext/unistd.h>` | `get_current_dir_name`, `getwd`, `getpt`, `closefrom`, `close_range` |
| `<muslext/wchar.h>` | `wcslcpy`, `wcslcat`, `wcstol_l`, `wcstoul_l` |
| `<fts.h>`, `<error.h>`, `<obstack.h>`, `<argz.h>` | fts, GNU error, obstack and argz |
| `<fstab.h>`, `<printf.h>`, `<sys/tree.h>`, `<sys/pidfd.h>` | fstab, printf hooks, tree macros and pidfd |

Use `<muslext/muslext.h>` for the complete interface surface.

## Feature-test macros

GNU extension interfaces require the consumer to define `_GNU_SOURCE`
before any system header:

- `<muslext/pthread_affinity.h>` requires `_GNU_SOURCE` for `cpu_set_t` and
  reports an explicit error otherwise; it declares `pthread_setaffinity_np`
  and `pthread_getaffinity_np`;
- `<muslext/setjmp.h>` requires `_GNU_SOURCE` or a POSIX feature macro for
  `sigjmp_buf`.

`<muslext/muslext.h>` includes these GNU-dependent headers only when the
corresponding feature macros expose their types.

## Interface notes

`fts` uses the standard glibc/musl-fts ABI, so ordinary programs
and tests compile without source changes.

`pthread_cancel` / `pthread_testcancel` are not provided; they cannot be
implemented correctly outside libc.

The `FNM_EXTMATCH` constant is defined, but extended matching is not
implemented yet.

## Out of scope

- gettext/libintl is intentionally not implemented; use an external gettext
  implementation.
- POSIX AIO, locale data, iconv tables and cryptographic algorithms are out
  of scope; use `libiconv`, `libxcrypt`, etc.

## Behavior notes

- The mount API, `ntp_adjtime`, `pidfd_*`, `getcpu`, `preadv64v2` /
  `pwritev64v2` depend on kernel support and fail with `ENOSYS` otherwise.
- `__libc_*`, `__fpu_control`, `__asinf_finite`, `__isinff` and `__isnanf`
  are best-effort ABI shims, not guarantees of libc-internal semantics.
- A-class wrappers (shadow, `pthread_setaffinity_np`/`pthread_getaffinity_np`, `statx`, etc.) depend on the runtime
  libc having a real implementation rather than an `ENOSYS` stub.
