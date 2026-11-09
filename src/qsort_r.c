/* qsort_r with the GNU comparator signature, clean-room implementation. */

#include <stddef.h>

typedef int (*compar_fn) (const void *, const void *, void *);

static void
swap_bytes (char *a, char *b, size_t size)
{
  size_t i;

  for (i = 0; i < size; i++)
    {
      char tmp = a[i];
      a[i] = b[i];
      b[i] = tmp;
    }
}

static void
sift_down (char *base, size_t root, size_t end, size_t size, compar_fn cmp,
           void *arg)
{
  for (;;)
    {
      size_t child = root * 2 + 1;
      size_t swap = root;

      if (child < end
          && cmp (base + swap * size, base + child * size, arg) < 0)
        swap = child;
      if (child + 1 < end
          && cmp (base + swap * size, base + (child + 1) * size, arg) < 0)
        swap = child + 1;
      if (swap == root)
        return;

      swap_bytes (base + root * size, base + swap * size, size);
      root = swap;
    }
}

void
qsort_r (void *base, size_t nmemb, size_t size,
         int (*compar) (const void *, const void *, void *), void *arg)
{
  char *data = (char *) base;
  size_t start;
  size_t end;

  if (nmemb < 2 || size == 0)
    return;

  for (start = nmemb / 2; start > 0; start--)
    sift_down (data, start - 1, nmemb, size, compar, arg);

  for (end = nmemb; end > 1; end--)
    {
      swap_bytes (data, data + (end - 1) * size, size);
      sift_down (data, 0, end - 1, size, compar, arg);
    }
}
