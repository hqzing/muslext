/* GNU obstack-compatible object stacks for systems without one. */

#ifndef MUSLEXT_OBSTACK_H
#define MUSLEXT_OBSTACK_H 1

#include <stdarg.h>
#include <stddef.h>
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

/* glibc <stdio.h> may define fortified redirects for these names. */
#ifdef obstack_printf
# undef obstack_printf
#endif
#ifdef obstack_vprintf
# undef obstack_vprintf
#endif

struct _obstack_chunk
{
  struct _obstack_chunk *prev;
  char *limit;
  char contents[1];
};

struct obstack
{
  struct _obstack_chunk *chunk;
  char *object_base;
  char *next_free;
  char *chunk_limit;
  size_t chunk_size;
  size_t alignment_mask;
  void *(*chunkfun) (size_t);
  void (*freefun) (void *);
};

int _obstack_begin (struct obstack *h, size_t size, size_t alignment,
                    void *(*chunkfun) (size_t), void (*freefun) (void *));
void _obstack_newchunk (struct obstack *h, size_t length);
void _obstack_free (struct obstack *h, void *obj);
void *_obstack_alloc (struct obstack *h, size_t size);
void _obstack_blank (struct obstack *h, size_t size);
void _obstack_grow (struct obstack *h, const void *data, size_t length);
void _obstack_grow0 (struct obstack *h, const void *data, size_t length);
void _obstack_1grow (struct obstack *h, int c);
void _obstack_int_grow (struct obstack *h, int value);
void *_obstack_finish (struct obstack *h);
void *_obstack_copy (struct obstack *h, const void *address, size_t length);
void *_obstack_copy0 (struct obstack *h, const void *address, size_t length);
int obstack_printf (struct obstack *h, const char *fmt, ...)
  __attribute__ ((__format__ (__printf__, 2, 3)));
int obstack_vprintf (struct obstack *h, const char *fmt, va_list ap);
int obstack_print (struct obstack *h, const char *string);
size_t _obstack_memory_used (struct obstack *h);

#define obstack_init(h) \
  _obstack_begin ((h), 0, 0, obstack_chunk_alloc, obstack_chunk_free)
#define obstack_begin(h, size) \
  _obstack_begin ((h), (size), 0, obstack_chunk_alloc, obstack_chunk_free)
#define obstack_specify_allocation(h, size, alignment, chunkfun, freefun) \
  _obstack_begin ((h), (size), (alignment), (chunkfun), (freefun))
#define obstack_alloc(h, size) _obstack_alloc ((h), (size))
#define obstack_blank(h, size) _obstack_blank ((h), (size))
#define obstack_grow(h, data, length) _obstack_grow ((h), (data), (length))
#define obstack_grow0(h, data, length) _obstack_grow0 ((h), (data), (length))
#define obstack_1grow(h, c) _obstack_1grow ((h), (c))
#define obstack_int_grow(h, value) _obstack_int_grow ((h), (value))
#define obstack_finish(h) _obstack_finish ((h))
#define obstack_copy(h, address, length) _obstack_copy ((h), (address), (length))
#define obstack_copy0(h, address, length) \
  _obstack_copy0 ((h), (address), (length))
#define obstack_free(h, obj) _obstack_free ((h), (obj))
#define obstack_object_size(h) \
  ((size_t) ((h)->next_free - (h)->object_base))
#define obstack_room(h) ((size_t) ((h)->chunk_limit - (h)->next_free))
#define obstack_alignment_mask(h) ((h)->alignment_mask)
#define obstack_memory_used(h) _obstack_memory_used ((h))
#define obstack_base(h) ((void *) (h)->object_base)
#define obstack_next_free(h) ((void *) (h)->next_free)

#ifdef __cplusplus
}
#endif

#endif /* MUSLEXT_OBSTACK_H */
