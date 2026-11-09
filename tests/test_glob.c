#include <assert.h>
#include <glob.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

#include <muslext/muslext.h>

static void
touch (const char *path)
{
  int fd = open (path, O_CREAT | O_WRONLY, 0644);
  assert (fd >= 0);
  close (fd);
}

int
main (void)
{
  char root[] = "/tmp/muslext-glob-XXXXXX";
  char pattern[512];
  glob_t result;
  int rc;

  assert (mkdtemp (root) != NULL);
  snprintf (pattern, sizeof (pattern), "%s/a.txt", root);
  touch (pattern);
  snprintf (pattern, sizeof (pattern), "%s/b.txt", root);
  touch (pattern);

  snprintf (pattern, sizeof (pattern), "%s/{a,b}.txt", root);
  memset (&result, 0, sizeof (result));
  rc = glob (pattern, GLOB_BRACE, NULL, &result);
  assert (rc == 0);
  assert (result.gl_pathc == 2);
  globfree (&result);

  snprintf (pattern, sizeof (pattern), "rm -rf '%s'", root);
  assert (system (pattern) == 0);
  return 0;
}
