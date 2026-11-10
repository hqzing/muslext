# Changelog

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
