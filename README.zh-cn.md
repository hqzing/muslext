<div align="center">
<p><a href="README.md">English</a> | 简体中文</p>
</div>

# muslext

muslext 是面向 musl libc 的兼容扩展库。它提供了一批常用扩展接口，让基于 glibc 的开源代码无需修改源码即可在基于 musl 的系统上编译运行。

本项目的主要目标平台是 OpenHarmony，同时也可用于其他基于 musl 的系统，例如 Alpine Linux。实际可用性取决于接口覆盖和运行环境。

## 构建

```sh
make -j"$(nproc)"
make install PREFIX="$HOME/.local"
```

安装目录由 `PREFIX` 决定，可以设为任意可写目录。安装内容包括 `lib/libmuslext.so`、`lib/libmuslext.a`、头文件和 `lib/pkgconfig/muslext.pc`。

## 使用

安装后，在代码中包含对应头文件即可。例如使用 `fts(3)`：

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
      /* 使用 entry */
    }
  fts_close(fts);
  return 0;
}
```

最直接的编译方式：

```sh
clang -I"$HOME/.local/include" app.c \
   -L"$HOME/.local/lib" -lmuslext -o app
```

对于需要 muslext 提供的额外声明/宏的代码，加上 `-include muslext/muslext.h`：

```sh
clang -include muslext/muslext.h -I"$HOME/.local/include" app.c \
   -L"$HOME/.local/lib" -lmuslext -o app
```

## 构建系统集成

推荐优先使用 pkg-config：

```sh
export PKG_CONFIG_PATH="$HOME/.local/lib/pkgconfig:${PKG_CONFIG_PATH:-}"
clang $(pkg-config --cflags --libs muslext) app.c -o app
```

如果构建系统只接受环境变量，使用标准的 `CPPFLAGS` / `LDFLAGS` / `LIBS`：

```sh
export CPPFLAGS="-I$HOME/.local/include ${CPPFLAGS:-}"
export LDFLAGS="-L$HOME/.local/lib ${LDFLAGS:-}"
export LIBS="-lmuslext ${LIBS:-}"
```

Autotools 和 Makefile 项目通常可以直接识别这三组变量；例如：

```sh
./configure CPPFLAGS="$CPPFLAGS" LDFLAGS="$LDFLAGS" LIBS="$LIBS"
make
```

`-lmuslext` 必须出现在对象文件之后、`-lc` 之前。

某些构建系统（例如 distutils）只允许通过 `LDFLAGS` 传链接参数，并把它们放在对象文件之前。此时需要显式关闭 as-needed，确保链接器保留 muslext：

```sh
export LDFLAGS="-L$HOME/.local/lib -Wl,--no-as-needed -lmuslext ${LDFLAGS:-}"
```

## 文档

- [原理与架构](docs/architecture.zh-cn.md)
- [兼容性与限制](docs/compatibility.zh-cn.md)
- [贡献指南](docs/contributing.zh-cn.md)
- [常见问题](docs/faq.zh-cn.md)

## 许可证

BSD Zero Clause License
