#include <assert.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <muslext/muslext.h>

int
main (void)
{
  char *cwd = get_current_dir_name ();
  char *resolved = canonicalize_file_name ("/");

  assert (cwd != NULL);
  assert (cwd[0] == '/');
  free (cwd);

  assert (resolved != NULL);
  assert (strcmp (resolved, "/") == 0);
  free (resolved);

  errno = 0;
  assert (reallocarray (NULL, (size_t) -1, 2) == NULL);
  assert (errno == ENOMEM);
  assert (pthread_yield () == 0);
  return 0;
}
