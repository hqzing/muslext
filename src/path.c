/* Path, descriptor and directory helpers. */

#define _GNU_SOURCE 1

#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/syscall.h>

#include <muslext/muslext.h>

char *
getwd (char *buf)
{
  if (buf == NULL)
    {
      errno = EINVAL;
      return NULL;
    }
  return getcwd (buf, PATH_MAX);
}

int
getpt (void)
{
  return posix_openpt (O_RDWR | O_NOCTTY);
}

void
closefrom (int lowfd)
{
#ifdef SYS_close_range
  if (syscall (SYS_close_range, (unsigned int) lowfd, ~0U, 0) == 0)
    return;
#endif
  {
    long limit = sysconf (_SC_OPEN_MAX);
    int fd;

    if (limit < 0)
      limit = 1024;
    for (fd = lowfd; fd < limit; fd++)
      close (fd);
  }
}

struct scandir_compar_ctx
{
  int (*compar) (const struct dirent **, const struct dirent **);
};

static int
scandir_compare (const void *left, const void *right, void *arg)
{
  struct scandir_compar_ctx *ctx = arg;

  return ctx->compar ((const struct dirent **) left,
                      (const struct dirent **) right);
}

int
scandirat (int dirfd, const char *dirp, struct dirent ***namelist,
           int (*filter) (const struct dirent *),
           int (*compar) (const struct dirent **, const struct dirent **))
{
  int fd;
  DIR *stream;
  struct dirent *entry;
  struct dirent **list = NULL;
  size_t count = 0;
  size_t allocated = 0;

  fd = openat (dirfd, dirp, O_RDONLY | O_DIRECTORY | O_CLOEXEC);
  if (fd < 0)
    return -1;
  stream = fdopendir (fd);
  if (stream == NULL)
    {
      close (fd);
      return -1;
    }

  while ((entry = readdir (stream)) != NULL)
    {
      struct dirent *copy;
      size_t length;

      if (filter != NULL && !filter (entry))
        continue;
      length = offsetof (struct dirent, d_name) + strlen (entry->d_name) + 1;
      copy = malloc (length);
      if (copy == NULL)
        goto fail;
      memcpy (copy, entry, length);
      if (count == allocated)
        {
          size_t next = allocated == 0 ? 16 : allocated * 2;
          struct dirent **grown = realloc (list, next * sizeof (*list));

          if (grown == NULL)
            {
              free (copy);
              goto fail;
            }
          list = grown;
          allocated = next;
        }
      list[count++] = copy;
    }
  closedir (stream);

  if (compar != NULL && count > 1)
    {
      struct scandir_compar_ctx ctx = { .compar = compar };

      qsort_r (list, count, sizeof (*list), scandir_compare, &ctx);
    }

  *namelist = list;
  return (int) count;

fail:
  closedir (stream);
  while (count > 0)
    free (list[--count]);
  free (list);
  errno = ENOMEM;
  return -1;
}
