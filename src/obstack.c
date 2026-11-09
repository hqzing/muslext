/* GNU obstack-compatible object stacks, clean-room implementation. */

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <obstack.h>

#define OBSTACK_ALIGN (sizeof (void *) * 2)
#define OBSTACK_DEFAULT_CHUNK 4096


int
_obstack_begin (struct obstack *h, size_t size, size_t alignment,
                void *(*chunkfun) (size_t), void (*freefun) (void *))
{
  size_t chunk_size = size ? size : OBSTACK_DEFAULT_CHUNK;
  struct _obstack_chunk *chunk;

  (void) alignment;

  chunk = chunkfun (sizeof (struct _obstack_chunk) + chunk_size);
  if (chunk == NULL)
    abort ();

  chunk->prev = NULL;
  chunk->limit = chunk->contents + chunk_size;

  h->chunk = chunk;
  h->object_base = chunk->contents;
  h->next_free = chunk->contents;
  h->chunk_limit = chunk->limit;
  h->chunk_size = chunk_size;
  h->alignment_mask = alignment != 0 ? alignment - 1 : sizeof (void *) - 1;
  h->chunkfun = chunkfun;
  h->freefun = freefun;

  return 1;
}

void
_obstack_newchunk (struct obstack *h, size_t length)
{
  size_t object_size = (size_t) (h->next_free - h->object_base);
  size_t needed = object_size + length + OBSTACK_ALIGN;
  size_t chunk_size = h->chunk_size ? h->chunk_size : OBSTACK_DEFAULT_CHUNK;
  struct _obstack_chunk *chunk;

  while (chunk_size < needed)
    chunk_size *= 2;

  chunk = h->chunkfun (sizeof (struct _obstack_chunk) + chunk_size);
  if (chunk == NULL)
    abort ();

  chunk->prev = h->chunk;
  chunk->limit = chunk->contents + chunk_size;

  if (object_size != 0)
    memcpy (chunk->contents, h->object_base, object_size);

  h->chunk = chunk;
  h->object_base = chunk->contents;
  h->next_free = chunk->contents + object_size;
  h->chunk_limit = chunk->limit;
  h->chunk_size = chunk_size;
}

void *
_obstack_alloc (struct obstack *h, size_t size)
{
  char *base;
  size_t padding = (size_t) (((uintptr_t) h->next_free) & h->alignment_mask);

  if (padding != 0)
    padding = h->alignment_mask + 1 - padding;

  if (size + padding > (size_t) (h->chunk_limit - h->next_free))
    {
      _obstack_newchunk (h, size + h->alignment_mask + 1);
      padding = (size_t) (((uintptr_t) h->next_free) & h->alignment_mask);
      if (padding != 0)
        padding = h->alignment_mask + 1 - padding;
    }

  base = h->next_free + padding;
  h->next_free = base + size;
  return base;
}

void
_obstack_blank (struct obstack *h, size_t size)
{
  (void) _obstack_alloc (h, size);
}

void
_obstack_grow (struct obstack *h, const void *data, size_t length)
{
  if (length > (size_t) (h->chunk_limit - h->next_free))
    _obstack_newchunk (h, length);

  if (length != 0)
    memcpy (h->next_free, data, length);
  h->next_free += length;
}

void
_obstack_grow0 (struct obstack *h, const void *data, size_t length)
{
  _obstack_grow (h, data, length);
  _obstack_1grow (h, '\0');
}

void
_obstack_1grow (struct obstack *h, int c)
{
  char value = (char) c;

  _obstack_grow (h, &value, 1);
}

void
_obstack_int_grow (struct obstack *h, int value)
{
  _obstack_grow (h, &value, sizeof (value));
}

void *
_obstack_finish (struct obstack *h)
{
  void *result = h->object_base;

  h->object_base = h->next_free;
  return result;
}

void *
_obstack_copy (struct obstack *h, const void *address, size_t length)
{
  void *result = _obstack_alloc (h, length);

  if (length != 0)
    memcpy (result, address, length);
  return result;
}

void *
_obstack_copy0 (struct obstack *h, const void *address, size_t length)
{
  void *result = _obstack_alloc (h, length + 1);

  if (length != 0)
    memcpy (result, address, length);
  ((char *) result)[length] = '\0';
  return result;
}

void
_obstack_free (struct obstack *h, void *obj)
{
  if (obj == NULL)
    {
      struct _obstack_chunk *chunk = h->chunk;

      while (chunk != NULL)
        {
          struct _obstack_chunk *prev = chunk->prev;
          h->freefun (chunk);
          chunk = prev;
        }

      h->chunk = NULL;
      h->object_base = NULL;
      h->next_free = NULL;
      h->chunk_limit = NULL;
      return;
    }

  struct _obstack_chunk *chunk = h->chunk;
  char *target = (char *) obj;

  while (chunk != NULL
         && !(target >= chunk->contents && target < chunk->limit))
    chunk = chunk->prev;

  if (chunk == NULL)
    abort ();

  while (h->chunk != chunk)
    {
      struct _obstack_chunk *prev = h->chunk->prev;
      h->freefun (h->chunk);
      h->chunk = prev;
    }

  h->object_base = target;
  h->next_free = target;
  h->chunk_limit = chunk->limit;
}

size_t
_obstack_memory_used (struct obstack *h)
{
  size_t total = 0;
  struct _obstack_chunk *chunk;

  for (chunk = h->chunk; chunk != NULL; chunk = chunk->prev)
    total += (size_t) (chunk->limit - chunk->contents);

  return total;
}
