/* strverscmp: compare strings with embedded version numbers. */

#define _GNU_SOURCE 1

#include <ctype.h>
#include <stddef.h>
#include <string.h>

#include <muslext/muslext.h>

#include "internal.h"

static int
version_compare (const char *left, const char *right)
{
  const unsigned char *a = (const unsigned char *) left;
  const unsigned char *b = (const unsigned char *) right;

  while (*a != '\0' && *b != '\0')
    {
      if (*a == *b)
        {
          a++;
          b++;
          continue;
        }

      if (isdigit (*a) && isdigit (*b))
        {
          const unsigned char *a_start = a;
          const unsigned char *b_start = b;
          size_t a_zeroes = 0;
          size_t b_zeroes = 0;
          size_t a_digits;
          size_t b_digits;
          int compared;

          while (*a == '0')
            a++, a_zeroes++;
          while (*b == '0')
            b++, b_zeroes++;
          a_start = a;
          b_start = b;
          while (isdigit (*a))
            a++;
          while (isdigit (*b))
            b++;
          a_digits = (size_t) (a - a_start);
          b_digits = (size_t) (b - b_start);

          if (a_digits != b_digits)
            return a_digits < b_digits ? -1 : 1;
          compared = memcmp (a_start, b_start, a_digits);
          if (compared != 0)
            return compared < 0 ? -1 : 1;

          if (a_zeroes != b_zeroes)
            return a_zeroes < b_zeroes ? -1 : 1;
        }

      return *a < *b ? -1 : 1;
    }

  if (*a != '\0')
    return 1;
  if (*b != '\0')
    return -1;
  return 0;
}

int
strverscmp (const char *left, const char *right)
{
  static int (*real) (const char *, const char *);

  if (real == NULL)
    real = (int (*)(const char *, const char *))
      muslext_resolve ("strverscmp", (void *) strverscmp);
  if (real != NULL)
    return real (left, right);
  return version_compare (left, right);
}
