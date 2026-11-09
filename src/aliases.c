/* glibc-internal symbol aliases and small compatibility shims. */

#define _GNU_SOURCE 1

#include <malloc.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

#include <muslext/muslext.h>

__attribute__((weak)) void *
__libc_realloc (void *ptr, size_t size)
{
  return realloc (ptr, size);
}

__attribute__((weak)) void *
__libc_memalign (size_t alignment, size_t size)
{
  return memalign (alignment, size);
}

__attribute__((weak)) float
__asinf_finite (float x)
{
  return asinf (x);
}

__attribute__((weak)) int
__isinff (float x)
{
  return isinf (x);
}

__attribute__((weak)) int
__isnanf (float x)
{
  return isnan (x);
}

__attribute__((weak)) unsigned long __fpu_control = 0;

const char *
gnu_get_libc_version (void)
{
  return "musl";
}

const char *
gnu_get_libc_release (void)
{
  return "stable";
}

int
malloc_trim (size_t pad)
{
  (void) pad;
  return 0;
}
