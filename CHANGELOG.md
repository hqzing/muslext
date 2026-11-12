# Changelog

## 1.5.0

This release contains source-level breaking changes.

### Changed

- Minimal headers no longer define `_GNU_SOURCE`.  Consumers must define the
  appropriate feature-test macros before any system header.
- `muslext/pthread.h` now contains only non-GNU pthread extensions; affinity
  moved to `muslext/pthread_affinity.h`, which requires `_GNU_SOURCE`.
- `muslext/muslext.h` includes GNU-dependent headers only when their types are
  visible.

## 1.4.0

### Added

- Minimal headers for the remaining compatibility interfaces:
  `pthread`, `stdlib`, `unistd`, `setjmp`, `string`, `crypt`, `wchar`,
  `math`, `signal`, `sys/sysmacros`, `resolv`, `glob`, `sys/mount`, `sched`,
  `sys/timex`, `sys/uio`, `malloc`, `fpu_control` and `gnu/libc-version`.

### Changed

- `muslext/dirent.h` now also declares `scandirat(3)`.
- `muslext/muslext.h` now includes all minimal headers instead of duplicating
  their declarations.

## 1.3.0

### Removed

- Remove the incomplete `argp` implementation. Programs that need `argp`
  should use a standalone argp implementation.

## 1.2.0

### Added

- `versionsort(3)`.

### Fixed

- Clear `errno` when `fts_read(3)` reaches EOF.

## 1.1.0

### Added

- Minimal headers for `getpass`, `utmpx`, `qsort_r`, shadow passwords and `statx`.

### Changed

- `muslext/muslext.h` now includes the minimal headers instead of duplicating
  their declarations.

The ABI version remains 1.

## 1.0.0

- First stable release.
