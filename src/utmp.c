/* utmpx database implementation, clean-room.
   Some musl runtimes export utmpx symbols that return ENOSYS; this file
   implements the file-backed database directly instead of delegating. */

#define _GNU_SOURCE 1

#include <errno.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <utmpx.h>
#include <sys/file.h>

#ifndef _PATH_UTMPX
# define _PATH_UTMPX "/var/run/utmp"
#endif

static char *utmp_path;
static int utmp_fd = -1;
static struct utmpx current_entry;

static const char *
path_or_default (void)
{
  return utmp_path != NULL ? utmp_path : _PATH_UTMPX;
}

static int
open_utmp (int flags)
{
  int fd = open (path_or_default (), flags, 0644);

  if (fd < 0)
    return -1;
  utmp_fd = fd;
  return 0;
}

static ssize_t
read_entry (struct utmpx *entry)
{
  ssize_t result;

  do
    result = read (utmp_fd, entry, sizeof (*entry));
  while (result < 0 && errno == EINTR);
  return result;
}

static ssize_t
write_entry (struct utmpx *entry)
{
  ssize_t result;

  do
    result = write (utmp_fd, entry, sizeof (*entry));
  while (result < 0 && errno == EINTR);
  return result;
}

void
updwtmpx (const char *file, const struct utmpx *entry)
{
  int fd;

  if (file == NULL || entry == NULL)
    return;
  fd = open (file, O_WRONLY | O_APPEND | O_CREAT, 0644);
  if (fd < 0)
    return;
  if (write (fd, entry, sizeof (*entry)) != (ssize_t) sizeof (*entry))
    {
      close (fd);
      return;
    }
  close (fd);
}

int
utmpxname (const char *file)
{
  char *copy;

  if (file == NULL)
    {
      errno = EINVAL;
      return -1;
    }

  copy = strdup (file);
  if (copy == NULL)
    return -1;

  free (utmp_path);
  utmp_path = copy;
  if (utmp_fd >= 0)
    {
      close (utmp_fd);
      utmp_fd = -1;
    }
  return 0;
}

void
setutxent (void)
{
  if (utmp_fd >= 0)
    close (utmp_fd);
  utmp_fd = -1;
  if (open_utmp (O_RDONLY) == 0)
    lseek (utmp_fd, 0, SEEK_SET);
}

void
endutxent (void)
{
  if (utmp_fd >= 0)
    close (utmp_fd);
  utmp_fd = -1;
}

struct utmpx *
getutxent (void)
{
  if (utmp_fd < 0 && open_utmp (O_RDONLY) < 0)
    return NULL;

  if (read_entry (&current_entry) != (ssize_t) sizeof (current_entry))
    return NULL;
  return &current_entry;
}

static int
id_matches (const struct utmpx *wanted, const struct utmpx *candidate)
{
  switch (wanted->ut_type)
    {
    case RUN_LVL:
    case BOOT_TIME:
    case NEW_TIME:
    case OLD_TIME:
      return wanted->ut_type == candidate->ut_type;
    default:
      return memcmp (wanted->ut_id, candidate->ut_id,
                     sizeof (wanted->ut_id)) == 0;
    }
}

struct utmpx *
getutxid (const struct utmpx *id)
{
  struct utmpx entry;

  if (id == NULL)
    {
      errno = EINVAL;
      return NULL;
    }
  if (utmp_fd < 0 && open_utmp (O_RDONLY) < 0)
    return NULL;

  while (read_entry (&entry) == (ssize_t) sizeof (entry))
    if (id_matches (id, &entry))
      {
        current_entry = entry;
        return &current_entry;
      }
  return NULL;
}

struct utmpx *
getutxline (const struct utmpx *line)
{
  struct utmpx entry;

  if (line == NULL)
    {
      errno = EINVAL;
      return NULL;
    }
  if (utmp_fd < 0 && open_utmp (O_RDONLY) < 0)
    return NULL;

  while (read_entry (&entry) == (ssize_t) sizeof (entry))
    if ((entry.ut_type == USER_PROCESS || entry.ut_type == LOGIN_PROCESS)
        && memcmp (entry.ut_line, line->ut_line,
                   sizeof (entry.ut_line)) == 0)
      {
        current_entry = entry;
        return &current_entry;
      }
  return NULL;
}

struct utmpx *
pututxline (const struct utmpx *entry)
{
  struct utmpx existing;
  off_t position;
  int created = 0;

  if (entry == NULL)
    {
      errno = EINVAL;
      return NULL;
    }

  if (utmp_fd >= 0)
    {
      close (utmp_fd);
      utmp_fd = -1;
    }
  if (open_utmp (O_RDWR) < 0)
    {
      if (open_utmp (O_RDWR | O_CREAT) < 0)
        return NULL;
      created = 1;
    }

  if (flock (utmp_fd, LOCK_EX) < 0)
    return NULL;

  int has_id = 0;
  size_t i;

  for (i = 0; i < sizeof (entry->ut_id); i++)
    if (entry->ut_id[i] != '\0')
      has_id = 1;

  position = 0;
  while (!created && has_id)
    {
      ssize_t got = read_entry (&existing);

      if (got != (ssize_t) sizeof (existing))
        break;
      position += (off_t) sizeof (existing);
      if (memcmp (existing.ut_id, entry->ut_id,
                  sizeof (entry->ut_id)) == 0)
        break;
    }

  if (!created && has_id && position >= (off_t) sizeof (existing)
      && memcmp (existing.ut_id, entry->ut_id, sizeof (entry->ut_id)) == 0)
    lseek (utmp_fd, position - (off_t) sizeof (existing), SEEK_SET);
  else
    lseek (utmp_fd, 0, SEEK_END);

  if (write_entry ((struct utmpx *) entry) != (ssize_t) sizeof (*entry))
    {
      flock (utmp_fd, LOCK_UN);
      return NULL;
    }

  fsync (utmp_fd);
  flock (utmp_fd, LOCK_UN);
  current_entry = *entry;
  return &current_entry;
}
