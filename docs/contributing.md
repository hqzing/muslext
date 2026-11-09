<div align="center">
<p>English | <a href="contributing.zh-cn.md">简体中文</a></p>
</div>

# Contributing

## Important: external pull requests are not accepted

muslext may be integrated by many users into distributions, artifacts,
SDKs or commercial products.  To let every downstream user rely on it
without legal uncertainty, the copyright provenance of this project must
remain absolutely clean.

This project therefore follows a model similar to SQLite:

- No external pull requests are accepted.
- Externally opened PRs are closed without review.
- All code changes are made by the maintainers.
- Please open an issue for bugs, missing interfaces or improvements; the
  maintainers will evaluate and implement them.

This is not meant to dismiss contributors.  It exists to ensure that:

1. every line can be traced to a maintainer's clean-room implementation;
2. no code of unknown origin, unknown license, or incompatible license is
   introduced;
3. downstream users can trust the copyright status unconditionally.

Please do not paste large amounts of third-party implementation code into
an issue.  Describe the required behaviour; the maintainers will implement
it clean-room.

## What you can submit

Issues are welcome for:

- build failures, runtime errors, ABI or behavioural incompatibilities;
- missing glibc/musl interfaces;
- a project or package that cannot build or run on a musl-based system;
- documentation or translation mistakes;

## What an issue should contain

- the muslext version (see `MUSLEXT_VERSION` in `include/muslext/muslext.h`)
- target platform and libc version;
- reproduction command and a minimal example;
- complete error output;
- for a package, its name and version;

## What the maintainers do

The maintainers will:

1. decide whether the request is in scope for muslext;
2. implement any code change clean-room;
3. update code, tests and documentation in the public repository;
4. report the result in the issue.
