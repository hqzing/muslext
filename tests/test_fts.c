#include <assert.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

#include <fts.h>

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
  char root[] = "/tmp/muslext-fts-XXXXXX";
  char path[512];
  char *paths[2];
  FTS *fts;
  FTSENT *entry;
  int saw_dir = 0;
  int saw_file = 0;
  int saw_dp = 0;

  assert (mkdtemp (root) != NULL);
  snprintf (path, sizeof (path), "%s/sub", root);
  assert (mkdir (path, 0755) == 0);
  snprintf (path, sizeof (path), "%s/sub/file", root);
  touch (path);

  paths[0] = root;
  paths[1] = NULL;
  fts = fts_open (paths, FTS_PHYSICAL, NULL);
  assert (fts != NULL);
  while ((entry = fts_read (fts)) != NULL)
    {
      if (entry->fts_info == FTS_D)
        saw_dir++;
      else if (entry->fts_info == FTS_F)
        saw_file++;
      else if (entry->fts_info == FTS_DP)
        saw_dp++;
    }
  assert (saw_dir == 2);
  assert (saw_file == 1);
  assert (saw_dp == 2);
  assert (fts_close (fts) == 0);

  {
    char link_path[512];
    snprintf (link_path, sizeof (link_path), "%s/link", root);
    assert (symlink ("sub", link_path) == 0);
    fts = fts_open (paths, FTS_LOGICAL, NULL);
    assert (fts != NULL);
    saw_file = 0;
    while ((entry = fts_read (fts)) != NULL)
      if (entry->fts_info == FTS_F && strcmp (entry->fts_name, "file") == 0)
        saw_file++;
    assert (saw_file == 2);
    fts_close (fts);
  }


  {
    FTSENT *child;

    fts = fts_open (paths, FTS_PHYSICAL, NULL);
    assert (fts != NULL);
    entry = fts_read (fts);
    assert (entry != NULL && entry->fts_info == FTS_D);
    child = fts_children (fts, 0);
    assert (child != NULL);
    assert (strcmp (child->fts_name, "sub") == 0 || strcmp (child->fts_name, "link") == 0);
    fts_close (fts);
  }

  {
    fts = fts_open (paths, FTS_PHYSICAL, NULL);
    assert (fts != NULL);
    assert (fts_read (fts)->fts_info == FTS_D);
    entry = fts_read (fts);
    assert (entry != NULL && entry->fts_info == FTS_D && strcmp (entry->fts_name, "sub") == 0);
    assert (fts_set (fts, entry, FTS_SKIP) == 0);
    while ((entry = fts_read (fts)) != NULL)
      assert (!(entry->fts_info == FTS_F && strcmp (entry->fts_name, "file") == 0));
    fts_close (fts);
  }

  snprintf (path, sizeof (path), "rm -rf '%s'", root);
  assert (system (path) == 0);
  return 0;
}
