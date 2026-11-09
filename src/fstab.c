/* fstab database, clean-room implementation. */

#define _GNU_SOURCE 1

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <fstab.h>

#ifndef _PATH_FSTAB
# define _PATH_FSTAB "/etc/fstab"
#endif

static FILE *fstab_stream;
static struct fstab current;
static char line_buffer[2048];
static char spec_buffer[512];
static char file_buffer[512];
static char vfstype_buffer[128];
static char mntops_buffer[512];

int
setfsent (void)
{
  if (fstab_stream != NULL)
    fclose (fstab_stream);
  fstab_stream = fopen (_PATH_FSTAB, "r");
  return fstab_stream != NULL;
}

void
endfsent (void)
{
  if (fstab_stream != NULL)
    fclose (fstab_stream);
  fstab_stream = NULL;
}

static void
copy_field (char *dest, size_t size, const char *source, const char *fallback)
{
  const char *value = source != NULL ? source : fallback;

  snprintf (dest, size, "%s", value != NULL ? value : "");
}

struct fstab *
getfsent (void)
{
  char *fields[6];
  int count;
  char *save = NULL;
  char *token;

  if (fstab_stream == NULL && !setfsent ())
    return NULL;

  for (;;)
    {
      if (fgets (line_buffer, sizeof (line_buffer), fstab_stream) == NULL)
        return NULL;
      line_buffer[strcspn (line_buffer, "\n")] = '\0';
      if (line_buffer[0] == '#' || line_buffer[0] == '\0')
        continue;

      count = 0;
      token = strtok_r (line_buffer, " \t", &save);
      while (token != NULL && count < 6)
        {
          fields[count++] = token;
          token = strtok_r (NULL, " \t", &save);
        }
      if (count < 2)
        continue;

      copy_field (spec_buffer, sizeof (spec_buffer), fields[0], "");
      copy_field (file_buffer, sizeof (file_buffer), fields[1], "");
      copy_field (vfstype_buffer, sizeof (vfstype_buffer),
                  count > 2 ? fields[2] : NULL, "defaults");
      copy_field (mntops_buffer, sizeof (mntops_buffer),
                  count > 3 ? fields[3] : NULL, "defaults");
      current.fs_spec = spec_buffer;
      current.fs_file = file_buffer;
      current.fs_vfstype = vfstype_buffer;
      current.fs_mntops = mntops_buffer;
      current.fs_type = strstr (mntops_buffer, "ro") != NULL
                        ? FSTAB_RO : FSTAB_RW;
      current.fs_freq = count > 4 ? atoi (fields[4]) : 0;
      current.fs_passno = count > 5 ? atoi (fields[5]) : 0;
      return &current;
    }
}

struct fstab *
getfsfile (const char *file)
{
  struct fstab *entry;

  if (!setfsent ())
    return NULL;
  while ((entry = getfsent ()) != NULL)
    if (strcmp (entry->fs_file, file) == 0)
      return entry;
  return NULL;
}

struct fstab *
getfsspec (const char *spec)
{
  struct fstab *entry;

  if (!setfsent ())
    return NULL;
  while ((entry = getfsent ()) != NULL)
    if (strcmp (entry->fs_spec, spec) == 0)
      return entry;
  return NULL;
}
