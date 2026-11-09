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
| Argp | `argp_parse`, `argp_help`, `argp_state_help`, `argp_error`, `argp_failure` |
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
| Password input | `getpass` |
| POSIX threads | `pthread_yield`, `pthread_tryjoin_np`, `pthread_timedjoin_np`, affinity |
| Credentials | `crypt_r`, shadow database delegation |
| Linux syscalls | mount API, `pidfd_open`, `pidfd_getfd`, `getcpu`, `ntp_adjtime`, `preadv64v2`, `pwritev64v2` |
| libc internals | `__libc_realloc`, `__libc_memalign`, `__asinf_finite`, `__isinff`, `__isnanf`, `__fpu_control`, `malloc_trim` |

## Interface notes

`fts` and `argp` use the standard glibc/musl-fts ABI, so ordinary programs
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
- A-class wrappers (shadow, affinity, `statx`, etc.) depend on the runtime
  libc having a real implementation rather than an `ENOSYS` stub.
