<div align="center">
<p>English | <a href="faq.zh-cn.md">简体中文</a></p>
</div>

# FAQ

## How is muslext pronounced?

Both readings are accepted:

- mus-lext: `/mʌsˈlɛkst/`
- musl-e-x-t: `/ˈmʌsəl iː ɛks tiː/`

Stress may vary between speakers. Neither reading is preferred.

## Are prebuilt binaries available?

Not currently.  muslext is distributed as source and built against the
target system's headers, sysroot and runtime libc.  A binary built for one
environment may not match another, and the project does not yet maintain a
binary compatibility matrix or a long-term ABI commitment for prebuilt
artifacts.  Publishing binaries now would make them look more portable than
they are.

The build is small, so running `make` from source with the exact toolchain
and flags is cheap.  This is a distribution policy, not a
claim that prebuilt binaries are impossible. Per-platform artifacts could be
added later together with a tested compatibility matrix.

## Can muslext be used in closed-source or commercial products?

Yes.  muslext is released under the BSD Zero Clause License (0BSD), a
permissive license.  It permits use, modification and redistribution,
including in closed-source products, without requiring attribution or
retention of copyright/license notices.
