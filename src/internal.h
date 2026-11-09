#ifndef MUSLEXT_INTERNAL_H
#define MUSLEXT_INTERNAL_H 1

#include <stddef.h>

/* Resolve NAME in a later object (the runtime libc).  SELF is the wrapper's
   own address; a pointer equal to SELF is never returned. */
void *muslext_resolve (const char *name, void *self);

/* Copy at most SIZE-1 bytes of SRC into DST, NUL terminate, return length. */
size_t muslext_strlcpy (char *dst, const char *src, size_t size);

#endif
