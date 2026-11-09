# Tests

Run the portable tests with:

```sh
make check
```

On a glibc host the shared library needs libcrypt:

```sh
make check LDLIBS="-lm -lcrypt"
```

Covered:

* `test_obstack` — growth, printf, chunk rollover, alignment masks
* `test_fts` — traversal, post-order, `fts_children`, `FTS_SKIP`, symlinks
* `test_qsort_r` — ascending/descending sorts with a context argument
* `test_error` — GNU error/error_at_line bookkeeping
* `test_misc` — cwd, realpath, reallocarray overflow, yield
* `test_delegate` — utmpx, shadow, strerror_r and program-name wrappers
* `test_tree` — red-black tree invariants under random insert/remove
* `test_gnu` — string/math/printf/argz/base64/glob/device helpers
* `test_argp` — argp option parsing
* `test_path` — getwd, scandirat, fstab
* `test_glob` — GLOB_BRACE expansion
* `test_getpass` — non-interactive password read
