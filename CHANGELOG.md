# Changelog

## 1.1.0

### Added

- Minimal headers for `getpass`, `utmpx`, `qsort_r`, shadow passwords and `statx`.

### Changed

- `muslext/muslext.h` now includes the minimal headers instead of duplicating
  their declarations.

The ABI version remains 1.

## 1.0.0

- First stable release.
