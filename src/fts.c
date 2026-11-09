/* fts(3) implementation, clean-room. */

#define _GNU_SOURCE 1

#include <dirent.h>
#include <errno.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

#include <fts.h>
#include <muslext/muslext.h>

struct fts_frame
{
  FTSENT *dir;
  FTSENT **entries;
  size_t count;
  size_t index;
  struct fts_frame *parent;
};

struct fts_internal
{
  FTSENT **roots;
  size_t root_count;
  size_t root_index;
  struct fts_frame *frame;
  FTSENT *pending;
  int skip_pending;
  FTSENT **child_list;
  size_t child_count;
};

static char *
xstrdup (const char *text)
{
  size_t length = strlen (text) + 1;
  char *copy = malloc (length);

  if (copy != NULL)
    memcpy (copy, text, length);
  return copy;
}

static char *
join_path (const char *dir, const char *name)
{
  size_t dir_len = strlen (dir);
  size_t name_len = strlen (name);

  while (name_len > 1 && name[name_len - 1] == '/')
    name_len--;
  int slash = dir_len > 0 && dir[dir_len - 1] != '/';
  char *path = malloc (dir_len + (size_t) slash + name_len + 1);

  if (path == NULL)
    return NULL;
  memcpy (path, dir, dir_len);
  if (slash)
    path[dir_len++] = '/';
  memcpy (path + dir_len, name, name_len + 1);
  return path;
}

static const char *
entry_name (const char *path)
{
  size_t length = strlen (path);
  const char *end;
  const char *slash = NULL;
  const char *p;

  while (length > 1 && path[length - 1] == '/')
    length--;
  end = path + length;

  for (p = path; p < end; p++)
    if (*p == '/')
      slash = p;

  if (slash == NULL)
    return path;
  if (slash + 1 == end)
    return "/";
  return slash + 1;
}

static FTSENT *
make_entry (FTS *sp, const char *path, const char *name, FTSENT *parent,
            size_t level)
{
  struct stat st;
  FTSENT *entry;
  size_t name_len = strlen (name);
  size_t size = sizeof (FTSENT) + name_len;
  int follow = (sp->fts_options & FTS_LOGICAL) != 0
               || (level == 0 && (sp->fts_options & FTS_COMFOLLOW) != 0);

  entry = calloc (1, size);
  if (entry == NULL)
    return NULL;

  entry->fts_path = xstrdup (path);
  if (entry->fts_path == NULL)
    {
      free (entry);
      return NULL;
    }

  memcpy (entry->fts_name, name, name_len + 1);
  entry->fts_namelen = name_len;
  entry->fts_pathlen = strlen (path);
  entry->fts_accpath = entry->fts_path;
  entry->fts_parent = parent;
  entry->fts_level = (unsigned short) level;
  entry->fts_statp = &entry->fts_stat;

  if ((follow ? stat (path, &st) : lstat (path, &st)) < 0)
    {
      if (follow && lstat (path, &st) == 0 && S_ISLNK (st.st_mode))
        entry->fts_info = FTS_SLNONE;
      else
        {
          entry->fts_info = FTS_NS;
          entry->fts_errno = errno;
        }
      return entry;
    }

  entry->fts_stat = st;
  entry->fts_dev = st.st_dev;
  entry->fts_ino = st.st_ino;
  entry->fts_nlink = st.st_nlink;

  if (S_ISDIR (st.st_mode))
    {
      FTSENT *ancestor;

      entry->fts_info = FTS_D;
      for (ancestor = parent; ancestor != NULL; ancestor = ancestor->fts_parent)
        if (ancestor->fts_info == FTS_D
            && ancestor->fts_dev == st.st_dev
            && ancestor->fts_ino == st.st_ino)
          {
            entry->fts_info = FTS_DC;
            entry->fts_cycle = ancestor;
            break;
          }
    }
  else if (S_ISLNK (st.st_mode))
    entry->fts_info = FTS_SL;
  else if (S_ISREG (st.st_mode))
    entry->fts_info = FTS_F;
  else
    entry->fts_info = FTS_DEFAULT;

  return entry;
}

static int
compare_entries (const void *left, const void *right, void *arg)
{
  FTS *sp = arg;
  FTSENT *a = *(FTSENT *const *) left;
  FTSENT *b = *(FTSENT *const *) right;

  return sp->fts_compar ((const FTSENT **) &a, (const FTSENT **) &b);
}

static void
free_entries (FTSENT **entries, size_t count)
{
  size_t i;

  if (entries == NULL)
    return;
  for (i = 0; i < count; i++)
    {
      free (entries[i]->fts_path);
      free (entries[i]);
    }
  free (entries);
}

static FTSENT **
read_directory (FTS *sp, FTSENT *dir, size_t *count)
{
  DIR *stream;
  struct dirent *de;
  FTSENT **entries = NULL;
  size_t used = 0;
  size_t allocated = 0;

  *count = 0;
  stream = opendir (dir->fts_accpath);
  if (stream == NULL)
    {
      dir->fts_errno = errno;
      return NULL;
    }

  while ((de = readdir (stream)) != NULL)
    {
      char *path;
      FTSENT *entry;

      if ((sp->fts_options & FTS_SEEDOT) == 0
          && (strcmp (de->d_name, ".") == 0 || strcmp (de->d_name, "..") == 0))
        continue;

      path = join_path (dir->fts_accpath, de->d_name);
      if (path == NULL)
        {
          closedir (stream);
          free_entries (entries, used);
          return NULL;
        }

      entry = make_entry (sp, path, de->d_name, dir, dir->fts_level + 1);
      free (path);
      if (entry == NULL)
        {
          closedir (stream);
          free_entries (entries, used);
          return NULL;
        }

      if (used == allocated)
        {
          size_t next = allocated == 0 ? 16 : allocated * 2;
          FTSENT **grown = realloc (entries, next * sizeof (*entries));

          if (grown == NULL)
            {
              free (entry->fts_path);
              free (entry);
              closedir (stream);
              free_entries (entries, used);
              return NULL;
            }
          entries = grown;
          allocated = next;
        }
      entries[used++] = entry;
    }
  closedir (stream);

  if (sp->fts_compar != NULL && used > 1)
    qsort_r (entries, used, sizeof (*entries), compare_entries, sp);

  *count = used;
  return entries;
}

static int
push_frame (FTS *sp, FTSENT *dir)
{
  struct fts_internal *internal = sp->fts_internal;
  struct fts_frame *frame = calloc (1, sizeof (*frame));

  if (frame == NULL)
    return -1;
  frame->dir = dir;
  frame->entries = read_directory (sp, dir, &frame->count);
  frame->parent = internal->frame;
  internal->frame = frame;
  return 0;
}

static void
pop_frame (FTS *sp)
{
  struct fts_internal *internal = sp->fts_internal;
  struct fts_frame *frame = internal->frame;

  if (frame == NULL)
    return;
  internal->frame = frame->parent;
  free_entries (frame->entries, frame->count);
  free (frame);
}

static void
free_child_list (struct fts_internal *internal)
{
  free_entries (internal->child_list, internal->child_count);
  internal->child_list = NULL;
  internal->child_count = 0;
}

FTS *
fts_open (char *const *path_argv, int options,
          int (*compar) (const FTSENT **, const FTSENT **))
{
  FTS *sp;
  struct fts_internal *internal;
  size_t count = 0;
  size_t i;

  if (path_argv == NULL || path_argv[0] == NULL)
    {
      errno = EINVAL;
      return NULL;
    }
  if ((options & FTS_LOGICAL) != 0 && (options & FTS_PHYSICAL) != 0)
    {
      errno = EINVAL;
      return NULL;
    }
  if ((options & (FTS_LOGICAL | FTS_PHYSICAL)) == 0)
    options |= FTS_PHYSICAL;

  while (path_argv[count] != NULL)
    count++;

  sp = calloc (1, sizeof (*sp));
  internal = calloc (1, sizeof (*internal));
  if (sp == NULL || internal == NULL)
    {
      free (sp);
      free (internal);
      return NULL;
    }

  internal->roots = calloc (count, sizeof (*internal->roots));
  if (internal->roots == NULL)
    {
      free (sp);
      free (internal);
      return NULL;
    }

  sp->fts_options = options;
  sp->fts_compar = compar;
  sp->fts_internal = internal;

  for (i = 0; i < count; i++)
    {
      const char *name = entry_name (path_argv[i]);

      internal->roots[i] = make_entry (sp, path_argv[i], name, NULL, 0);
      if (internal->roots[i] == NULL)
        {
          size_t j;

          for (j = 0; j < i; j++)
            {
              free (internal->roots[j]->fts_path);
              free (internal->roots[j]);
            }
          free (internal->roots);
          free (internal);
          free (sp);
          return NULL;
        }
      internal->root_count++;
    }

  return sp;
}

FTSENT *
fts_read (FTS *sp)
{
  struct fts_internal *internal = sp->fts_internal;

  free_child_list (internal);

  for (;;)
    {
      if (internal->pending != NULL)
        {
          FTSENT *directory = internal->pending;
          int descend = !internal->skip_pending
                        && directory->fts_info == FTS_D
                        && !((sp->fts_options & FTS_XDEV) != 0
                             && directory->fts_parent != NULL
                             && directory->fts_dev
                                != directory->fts_parent->fts_dev);

          internal->pending = NULL;
          internal->skip_pending = 0;
          if (descend)
            push_frame (sp, directory);
        }

      if (internal->frame != NULL)
        {
          struct fts_frame *frame = internal->frame;

          if (frame->index < frame->count)
            {
              FTSENT *entry = frame->entries[frame->index++];

              sp->fts_cur = entry;
              if (entry->fts_info == FTS_D)
                internal->pending = entry;
              return entry;
            }

          {
            FTSENT *directory = frame->dir;

            pop_frame (sp);
            if (directory != NULL)
              {
                directory->fts_info = FTS_DP;
                sp->fts_cur = directory;
                return directory;
              }
          }
          continue;
        }

      if (internal->root_index < internal->root_count)
        {
          FTSENT *entry = internal->roots[internal->root_index++];

          sp->fts_cur = entry;
          if (entry->fts_info == FTS_D)
            internal->pending = entry;
          return entry;
        }

      sp->fts_cur = NULL;
      return NULL;
    }
}

FTSENT *
fts_children (FTS *sp, int options)
{
  struct fts_internal *internal = sp->fts_internal;
  FTSENT *directory = sp->fts_cur;
  FTSENT **entries;
  size_t count;
  size_t i;

  (void) options;

  if (directory == NULL
      || (directory->fts_info != FTS_D && directory->fts_info != FTS_DP))
    {
      errno = ENOTDIR;
      return NULL;
    }

  free_child_list (internal);
  entries = read_directory (sp, directory, &count);
  if (entries == NULL)
    return NULL;

  for (i = 0; i + 1 < count; i++)
    entries[i]->fts_link = entries[i + 1];
  if (count != 0)
    entries[count - 1]->fts_link = NULL;

  internal->child_list = entries;
  internal->child_count = count;
  return count != 0 ? entries[0] : NULL;
}

int
fts_set (FTS *sp, FTSENT *entry, int options)
{
  struct fts_internal *internal = sp->fts_internal;

  if (entry != sp->fts_cur)
    {
      errno = EINVAL;
      return -1;
    }

  switch (options)
    {
    case FTS_SKIP:
      if (entry->fts_info == FTS_D && internal->pending == entry)
        internal->skip_pending = 1;
      return 0;

    case FTS_FOLLOW:
      if (entry->fts_info == FTS_SL || entry->fts_info == FTS_SLNONE)
        {
          struct stat st;

          if (stat (entry->fts_accpath, &st) == 0)
            {
              entry->fts_stat = st;
              entry->fts_dev = st.st_dev;
              entry->fts_ino = st.st_ino;
              entry->fts_nlink = st.st_nlink;
              entry->fts_info = S_ISDIR (st.st_mode) ? FTS_D
                              : S_ISREG (st.st_mode) ? FTS_F
                              : FTS_DEFAULT;
              if (entry->fts_info == FTS_D)
                internal->pending = entry;
            }
        }
      return 0;

    case FTS_AGAIN:
      if (internal->frame != NULL && internal->frame->index > 0)
        internal->frame->index--;
      return 0;

    case FTS_NOINSTR:
      return 0;

    default:
      errno = EINVAL;
      return -1;
    }
}

int
fts_close (FTS *sp)
{
  struct fts_internal *internal = sp->fts_internal;

  if (internal != NULL)
    {
      while (internal->frame != NULL)
        pop_frame (sp);
      free_child_list (internal);
      free_entries (internal->roots, internal->root_count);
      free (internal);
    }
  free (sp->fts_path);
  free (sp);
  return 0;
}
