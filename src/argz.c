/* GNU argz vectors, clean-room implementation. */

#define _GNU_SOURCE 1

#include <errno.h>
#include <stdlib.h>
#include <string.h>

#include <argz.h>

static error_t
resize (char **argz, size_t *argz_len, size_t new_len)
{
  char *grown = realloc (*argz, new_len);

  if (grown == NULL)
    return ENOMEM;
  *argz = grown;
  *argz_len = new_len;
  return 0;
}

error_t
argz_create (char *const argv[], char **argz, size_t *argz_len)
{
  size_t total = 1;
  size_t i;
  size_t offset = 0;
  char *result;

  if (argv == NULL)
    {
      *argz = NULL;
      *argz_len = 0;
      return 0;
    }
  for (i = 0; argv[i] != NULL; i++)
    total += strlen (argv[i]) + 1;

  result = malloc (total);
  if (result == NULL)
    return ENOMEM;
  for (i = 0; argv[i] != NULL; i++)
    {
      size_t length = strlen (argv[i]) + 1;

      memcpy (result + offset, argv[i], length);
      offset += length;
    }
  result[offset] = '\0';
  *argz = result;
  *argz_len = total;
  return 0;
}

error_t
argz_create_sep (const char *string, int sep, char **argz, size_t *argz_len)
{
  size_t length = strlen (string);
  char *result = malloc (length + 2);
  size_t i;
  size_t out = 0;

  if (result == NULL)
    return ENOMEM;

  for (i = 0; i <= length; i++)
    {
      if (string[i] == sep || string[i] == '\0')
        {
          result[out++] = '\0';
          if (string[i] == '\0')
            break;
        }
      else
        result[out++] = string[i];
    }
  result[out] = '\0';
  *argz = result;
  *argz_len = out + 1;
  return 0;
}

size_t
argz_count (const char *argz, size_t argz_len)
{
  size_t count = 0;
  size_t i;

  for (i = 0; i < argz_len; i++)
    if (argz[i] == '\0')
      count++;
  return count > 0 ? count - 1 : 0;
}

void
argz_stringify (char *argz, size_t argz_len, int sep)
{
  size_t i;

  for (i = 0; i + 2 < argz_len; i++)
    if (argz[i] == '\0')
      argz[i] = (char) sep;
}

error_t
argz_add (char **argz, size_t *argz_len, const char *str)
{
  size_t length = strlen (str) + 1;
  size_t old = *argz_len;
  error_t error;

  if (old == 0)
    {
      char *result = malloc (length + 1);

      if (result == NULL)
        return ENOMEM;
      memcpy (result, str, length);
      result[length] = '\0';
      *argz = result;
      *argz_len = length + 1;
      return 0;
    }

  error = resize (argz, argz_len, old + length);
  if (error != 0)
    return error;
  memcpy (*argz + old - 1, str, length);
  (*argz)[old + length - 1] = '\0';
  return 0;
}

error_t
argz_append (char **argz, size_t *argz_len, const char *buf, size_t buf_len)
{
  size_t old = *argz_len;
  error_t error;

  if (buf_len == 0)
    return 0;
  if (old == 0)
    {
      char *result = malloc (buf_len + 1);

      if (result == NULL)
        return ENOMEM;
      memcpy (result, buf, buf_len);
      result[buf_len] = '\0';
      *argz = result;
      *argz_len = buf_len + 1;
      return 0;
    }

  error = resize (argz, argz_len, old + buf_len);
  if (error != 0)
    return error;
  memcpy (*argz + old - 1, buf, buf_len);
  (*argz)[old + buf_len - 1] = '\0';
  return 0;
}

error_t
argz_insert (char **argz, size_t *argz_len, char *before, const char *entry)
{
  size_t length = strlen (entry) + 1;
  size_t old = *argz_len;
  size_t offset;
  error_t error;

  if (old == 0 || before == NULL)
    return argz_add (argz, argz_len, entry);
  if (before < *argz || before >= *argz + old)
    return EINVAL;

  offset = (size_t) (before - *argz);
  error = resize (argz, argz_len, old + length);
  if (error != 0)
    return error;
  memmove (*argz + offset + length, *argz + offset, old - offset);
  memcpy (*argz + offset, entry, length);
  (*argz)[old + length - 1] = '\0';
  return 0;
}

error_t
argz_add_sep (char **argz, size_t *argz_len, const char *string, int delim)
{
  char *copy = strdup (string);
  char *start;
  char *p;
  error_t error = 0;

  if (copy == NULL)
    return ENOMEM;
  start = copy;
  for (p = copy; ; p++)
    {
      if (*p == delim || *p == '\0')
        {
          int end = *p == '\0';
          *p = '\0';
          if (*start != '\0')
            {
              error = argz_add (argz, argz_len, start);
              if (error != 0)
                break;
            }
          if (end)
            break;
          start = p + 1;
        }
    }
  free (copy);
  return error;
}
