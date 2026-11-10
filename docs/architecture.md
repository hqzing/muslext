<div align="center">
<p>English | <a href="architecture.zh-cn.md">简体中文</a></p>
</div>

# Architecture

## Two classes

| Class | Meaning | Examples |
| --- | --- | --- |
| A | The runtime libc exports the symbol, but the build environment's headers or link information do not expose it. muslext forwards to the real function with `dlsym(RTLD_NEXT)`. | `statx`, `__xpg_strerror_r`, `pthread_setaffinity_np` |
| B | Neither the runtime libc nor the build environment provides it; muslext carries an implementation. | `fts(3)`, GNU `obstack`, `error.h`, `utmpx` |

## Class A: runtime delegation

A-class wrappers live in `src/`:

1. resolve the real libc function with `dlsym(RTLD_NEXT, "name")`;
2. fall back to `dlsym(RTLD_DEFAULT, "name")`;
3. never call themselves recursively;
4. if the runtime libc only has an `ENOSYS` stub, the wrapper can only fail;
   an extension library cannot repair that.

## Class B: self-contained implementations

B-class interfaces are implemented in `src/` without relying on glibc
source or behaviour.  They follow the public POSIX/GNU interface semantics
and cover:

- `fts(3)` directory traversal;
- GNU `obstack`;
- GNU `error.h`;
- the `utmpx` file database;
- `argz`, `fstab`, `glob`, `getpass`;
- commonly missing interfaces such as `qsort_r`, `crypt_r` and
  `strverscmp`.
