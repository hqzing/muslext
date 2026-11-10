/* versionsort(3) orders names with embedded numbers. */

#include <assert.h>
#include <dirent.h>
#include <string.h>

#include <muslext/dirent.h>

int
main (void)
{
  struct dirent two;
  struct dirent ten;
  const struct dirent *a = &two;
  const struct dirent *b = &ten;

  memset (&two, 0, sizeof two);
  memset (&ten, 0, sizeof ten);
  strcpy (two.d_name, "file2");
  strcpy (ten.d_name, "file10");

  assert (versionsort (&a, &b) < 0);
  assert (versionsort (&b, &a) > 0);
  assert (versionsort (&a, &a) == 0);

  return 0;
}
