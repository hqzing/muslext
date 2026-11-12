/* Minimal malloc extensions provided by muslext. */

#ifndef MUSLEXT_MALLOC_H
#define MUSLEXT_MALLOC_H 1

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

void *__libc_realloc (void *ptr, size_t size);
void *__libc_memalign (size_t alignment, size_t size);
int malloc_trim (size_t pad);

#ifdef __cplusplus
}
#endif

#endif /* MUSLEXT_MALLOC_H */
