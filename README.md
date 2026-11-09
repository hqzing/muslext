<div align="center">
<p>English | <a href="README.zh-cn.md">简体中文</a></p>
</div>

# muslext

muslext is a compatibility extension library for musl libc.  It provides a
set of commonly used extension interfaces, letting glibc-based open-source
code build and run without source changes on musl-based systems.

Its primary target platform is OpenHarmony.  It can also be used on other
musl-based systems, such as Alpine Linux.  Actual availability depends on
interface coverage and the runtime environment.

## Build

```sh
make -j"$(nproc)"
make install PREFIX="$HOME/.local"
```

`PREFIX` can be any writable directory.  The installation provides
`lib/libmuslext.so`, `lib/libmuslext.a`, the headers and
`lib/pkgconfig/muslext.pc`.

## Usage

After installation, include the header for the interface you use.  For
example, `fts(3)`:

```c
#include <fts.h>

int main(void)
{
  char *paths[] = { ".", NULL };
  FTS *fts = fts_open(paths, FTS_PHYSICAL, NULL);
  if (fts == NULL)
    return 1;

  FTSENT *entry;
  while ((entry = fts_read(fts)) != NULL)
    {
      /* use entry */
    }
  fts_close(fts);
  return 0;
}
```

The simplest compile command:

```sh
clang -I"$HOME/.local/include" app.c \
   -L"$HOME/.local/lib" -lmuslext -o app
```

For code that needs the extra declarations/macros provided by muslext, add `-include muslext/muslext.h`:

```sh
clang -include muslext/muslext.h -I"$HOME/.local/include" app.c \
   -L"$HOME/.local/lib" -lmuslext -o app
```

## Build-system integration

pkg-config is the preferred route:

```sh
export PKG_CONFIG_PATH="$HOME/.local/lib/pkgconfig:${PKG_CONFIG_PATH:-}"
clang $(pkg-config --cflags --libs muslext) app.c -o app
```

If the build system only accepts environment variables, use the standard
`CPPFLAGS` / `LDFLAGS` / `LIBS`:

```sh
export CPPFLAGS="-I$HOME/.local/include ${CPPFLAGS:-}"
export LDFLAGS="-L$HOME/.local/lib ${LDFLAGS:-}"
export LIBS="-lmuslext ${LIBS:-}"
```

Autotools and Makefile projects normally consume these three variables
directly, for example:

```sh
./configure CPPFLAGS="$CPPFLAGS" LDFLAGS="$LDFLAGS" LIBS="$LIBS"
make
```

`-lmuslext` must come after the object files and before `-lc`.

Some build systems (for example distutils) only accept link flags through
`LDFLAGS` and place them before the object files.  In that case, disable
as-needed explicitly so the linker keeps muslext:

```sh
export LDFLAGS="-L$HOME/.local/lib -Wl,--no-as-needed -lmuslext ${LDFLAGS:-}"
```

## Documentation

- [Architecture](docs/architecture.md)
- [Compatibility and limitations](docs/compatibility.md)
- [Contributing guide](docs/contributing.md)
- [FAQ](docs/faq.md)

## License

BSD Zero Clause License
