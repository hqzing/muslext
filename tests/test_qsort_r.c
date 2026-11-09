#include <assert.h>
#include <stddef.h>
#include <stdlib.h>

#include <muslext/muslext.h>

static int
compare (const void *left, const void *right, void *arg)
{
  int a = *(const int *) left;
  int b = *(const int *) right;
  int direction = *(int *) arg;

  if (a < b)
    return -direction;
  if (a > b)
    return direction;
  return 0;
}

int
main (void)
{
  int values[] = { 5, 1, 9, 3, 7, 3, 8, 2 };
  int direction = 1;
  size_t i;

  qsort_r (values, sizeof (values) / sizeof (values[0]), sizeof (values[0]),
           compare, &direction);
  for (i = 1; i < sizeof (values) / sizeof (values[0]); i++)
    assert (values[i - 1] <= values[i]);

  direction = -1;
  qsort_r (values, sizeof (values) / sizeof (values[0]), sizeof (values[0]),
           compare, &direction);
  for (i = 1; i < sizeof (values) / sizeof (values[0]); i++)
    assert (values[i - 1] >= values[i]);
  return 0;
}
