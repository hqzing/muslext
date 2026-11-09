/* obstack_printf/vprintf/print, implemented with vsnprintf. */

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <obstack.h>

int
obstack_vprintf (struct obstack *h, const char *fmt, va_list ap)
{
  va_list ap2;
  char *buffer;
  int length;

  va_copy (ap2, ap);
  length = vsnprintf (NULL, 0, fmt, ap2);
  va_end (ap2);

  if (length < 0)
    return length;

  buffer = malloc ((size_t) length + 1);
  if (buffer == NULL)
    return -1;

  vsnprintf (buffer, (size_t) length + 1, fmt, ap);
  _obstack_grow (h, buffer, (size_t) length);
  free (buffer);

  return length;
}

int
obstack_printf (struct obstack *h, const char *fmt, ...)
{
  va_list ap;
  int result;

  va_start (ap, fmt);
  result = obstack_vprintf (h, fmt, ap);
  va_end (ap);

  return result;
}

int
obstack_print (struct obstack *h, const char *string)
{
  size_t length = strlen (string);

  _obstack_grow (h, string, length);
  return (int) length;
}
