/* glob(3) wrappers for GLOB_BRACE and GLOB_TILDE. */

#define _GNU_SOURCE 1

#include <errno.h>
#include <glob.h>
#include <pwd.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <muslext/muslext.h>

#include "internal.h"

static int
append_pattern (char ***patterns, size_t *count, size_t *allocated, char *pattern)
{
  if (*count == *allocated)
    {
      size_t next = *allocated == 0 ? 8 : *allocated * 2;
      char **grown = realloc (*patterns, next * sizeof (**patterns));

      if (grown == NULL)
        {
          free (pattern);
          return -1;
        }
      *patterns = grown;
      *allocated = next;
    }
  (*patterns)[(*count)++] = pattern;
  return 0;
}

static const char *
find_closing_brace (const char *open)
{
  int depth = 0;

  for (const char *p = open; *p != '\0'; p++)
    {
      if (*p == '\\' && p[1] != '\0')
        {
          p++;
          continue;
        }
      if (*p == '{')
        depth++;
      else if (*p == '}')
        {
          depth--;
          if (depth == 0)
            return p;
        }
    }
  return NULL;
}

static int
expand_braces (const char *pattern, char ***out, size_t *out_count)
{
  const char *open = NULL;
  const char *close;
  size_t allocated = 0;

  for (const char *p = pattern; *p != '\0'; p++)
    {
      if (*p == '\\' && p[1] != '\0')
        {
          p++;
          continue;
        }
      if (*p == '{')
        {
          open = p;
          break;
        }
    }

  if (open == NULL)
    return append_pattern (out, out_count, &allocated, strdup (pattern));

  close = find_closing_brace (open);
  if (close == NULL)
    return append_pattern (out, out_count, &allocated, strdup (pattern));

  {
    const char *start = open + 1;
    int depth = 0;

    for (const char *p = start; p <= close; p++)
      {
        if (p < close && *p == '\\' && p[1] != '\0')
          {
            p++;
            continue;
          }
        if (p == close || (*p == ',' && depth == 0))
          {
            char *piece = malloc ((size_t) (p - start) + 1);
            char *combined;
            char **sub = NULL;
            size_t sub_count = 0;

            if (piece == NULL)
              return -1;
            memcpy (piece, start, (size_t) (p - start));
            piece[p - start] = '\0';
            combined = malloc ((size_t) (open - pattern)
                               + strlen (piece) + strlen (close + 1) + 1);
            if (combined == NULL)
              {
                free (piece);
                return -1;
              }
            memcpy (combined, pattern, (size_t) (open - pattern));
            strcpy (combined + (open - pattern), piece);
            strcat (combined, close + 1);
            free (piece);
            if (expand_braces (combined, &sub, &sub_count) != 0)
              {
                free (combined);
                return -1;
              }
            free (combined);
            for (size_t i = 0; i < sub_count; i++)
              if (append_pattern (out, out_count, &allocated, sub[i]) != 0)
                {
                  for (size_t j = i + 1; j < sub_count; j++)
                    free (sub[j]);
                  free (sub);
                  return -1;
                }
            free (sub);
            if (p == close)
              break;
            start = p + 1;
          }
        else if (*p == '{')
          depth++;
        else if (*p == '}')
          depth--;
      }
  }
  return 0;
}

static char *
expand_tilde (const char *pattern)
{
  const char *slash;
  const char *user;
  size_t user_len;
  const char *home = NULL;
  struct passwd *entry;
  char *result;

  if (pattern[0] != '~')
    return strdup (pattern);
  slash = strchr (pattern, '/');
  user = pattern + 1;
  user_len = slash != NULL ? (size_t) (slash - user) : strlen (user);

  if (user_len == 0)
    {
      home = getenv ("HOME");
      if (home == NULL)
        {
          entry = getpwuid (getuid ());
          home = entry != NULL ? entry->pw_dir : NULL;
        }
    }
  else
    {
      char name[256];

      if (user_len >= sizeof (name))
        return NULL;
      memcpy (name, user, user_len);
      name[user_len] = '\0';
      entry = getpwnam (name);
      home = entry != NULL ? entry->pw_dir : NULL;
    }
  if (home == NULL)
    return strdup (pattern);
  if (slash == NULL)
    return strdup (home);
  result = malloc (strlen (home) + strlen (slash) + 1);
  if (result != NULL)
    {
      strcpy (result, home);
      strcat (result, slash);
    }
  return result;
}

int
glob (const char *pattern, int flags, int (*errfunc) (const char *, int),
      glob_t *pglob)
{
  static int (*real) (const char *, int, int (*)(const char *, int), glob_t *);
  char *expanded;
  char **patterns = NULL;
  size_t pattern_count = 0;
  int base_flags;
  int result = 0;

  if (pglob == NULL)
    {
      errno = EINVAL;
      return GLOB_NOSPACE;
    }

  if (real == NULL)
    real = (int (*)(const char *, int, int (*)(const char *, int), glob_t *))
      muslext_resolve ("glob", (void *) glob);
  if (real == NULL)
    return GLOB_NOSPACE;

  if ((flags & (GLOB_BRACE | GLOB_TILDE | GLOB_TILDE_CHECK)) == 0)
    return real (pattern, flags, errfunc, pglob);

  expanded = (flags & (GLOB_TILDE | GLOB_TILDE_CHECK)) != 0
             ? expand_tilde (pattern) : strdup (pattern);
  if (expanded == NULL)
    return GLOB_NOSPACE;

  if ((flags & GLOB_BRACE) != 0)
    {
      if (expand_braces (expanded, &patterns, &pattern_count) != 0)
        {
          free (expanded);
          return GLOB_NOSPACE;
        }
      free (expanded);
    }
  else
    {
      patterns = malloc (sizeof (*patterns));
      if (patterns == NULL)
        {
          free (expanded);
          return GLOB_NOSPACE;
        }
      patterns[0] = expanded;
      pattern_count = 1;
    }

  base_flags = flags & ~(GLOB_BRACE | GLOB_TILDE | GLOB_TILDE_CHECK);
  for (size_t i = 0; i < pattern_count; i++)
    {
      int sub_flags = base_flags;

      if (i != 0 || (flags & GLOB_APPEND) != 0)
        sub_flags |= GLOB_APPEND;
      result = real (patterns[i], sub_flags, errfunc, pglob);
      if (result == GLOB_NOMATCH && (flags & GLOB_NOCHECK) != 0)
        result = 0;
      if (result != 0)
        break;
    }

  for (size_t i = 0; i < pattern_count; i++)
    free (patterns[i]);
  free (patterns);
  return result;
}
