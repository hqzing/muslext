/* Minimal declaration of the glibc-compatible qsort_r(3). */

#ifndef MUSLEXT_QSORT_R_H
#define MUSLEXT_QSORT_R_H 1

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

void qsort_r (void *base, size_t nmemb, size_t size,
              int (*compar) (const void *, const void *, void *), void *arg);

#ifdef __cplusplus
}
#endif

#endif /* MUSLEXT_QSORT_R_H */
