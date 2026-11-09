#include <assert.h>
#include <dirent.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>

#include <fstab.h>
#include <muslext/muslext.h>

static int
filter (const struct dirent *entry)
{
  return entry->d_name[0] != '.';
}

int
main (void)
{
  char cwd[4096];
  char root[] = "/tmp/muslext-path-XXXXXX";
  struct dirent **entries;
  int count;
  int fd;

  assert (getwd (cwd) == cwd);
  assert (cwd[0] == '/');

  assert (mkdtemp (root) != NULL);
  fd = open (root, O_RDONLY | O_DIRECTORY);
  assert (fd >= 0);
  count = scandirat (fd, ".", &entries, filter, alphasort);
  assert (count == 0);
  close (fd);

  assert (getpt () >= 0 || errno == ENOENT || errno == EACCES);

  {
    int found = 0;
    if (setfsent ())
      {
        while (getfsent () != NULL)
          found++;
        endfsent ();
      }
    (void) found;
  }

  {
    char command[512];
    snprintf (command, sizeof (command), "rm -rf '%s'", root);
    assert (system (command) == 0);
  }
  return 0;
}
