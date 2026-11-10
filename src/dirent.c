/* GNU versionsort(3): compare directory entries by version-aware name. */

#define _GNU_SOURCE 1

#include <dirent.h>

#include <muslext/muslext.h>

int
versionsort (const struct dirent **a, const struct dirent **b)
{
  return strverscmp ((*a)->d_name, (*b)->d_name);
}
