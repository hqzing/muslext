#define _GNU_SOURCE 1
#include <dlfcn.h>

#include "internal.h"

void *
muslext_resolve (const char *name, void *self)
{
  void *symbol = dlsym (RTLD_NEXT, name);

  if (symbol == NULL || symbol == self)
    symbol = dlsym (RTLD_DEFAULT, name);
  if (symbol == self)
    symbol = NULL;
  return symbol;
}

size_t
muslext_strlcpy (char *dst, const char *src, size_t size)
{
  size_t length = 0;

  if (size != 0)
    {
      while (src[length] != '\0' && length + 1 < size)
        {
          dst[length] = src[length];
          length++;
        }
      dst[length] = '\0';
    }
  while (src[length] != '\0')
    length++;
  return length;
}
