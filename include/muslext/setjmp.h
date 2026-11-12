/* Minimal setjmp extensions provided by muslext. */

#ifndef MUSLEXT_SETJMP_H
#define MUSLEXT_SETJMP_H 1

#ifndef _GNU_SOURCE
# define _GNU_SOURCE 1
#endif
#include <setjmp.h>

#ifdef __cplusplus
extern "C" {
#endif

int __sigsetjmp (sigjmp_buf env, int savemask);

#ifdef __cplusplus
}
#endif

#endif /* MUSLEXT_SETJMP_H */
