/* Minimal setjmp extensions provided by muslext. */

#ifndef MUSLEXT_SETJMP_H
#define MUSLEXT_SETJMP_H 1

#include <setjmp.h>

#if !defined(_GNU_SOURCE) && !defined(_POSIX_SOURCE) \
    && !defined(_POSIX_C_SOURCE) && !defined(_XOPEN_SOURCE) \
    && !defined(_BSD_SOURCE)
# error "muslext/setjmp.h requires POSIX or GNU feature-test macros"
#endif

#ifdef __cplusplus
extern "C" {
#endif

int __sigsetjmp (sigjmp_buf env, int savemask);

#ifdef __cplusplus
}
#endif

#endif /* MUSLEXT_SETJMP_H */
