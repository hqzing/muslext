/* GNU error.h implementation. */

#include <errno.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <error.h>

unsigned int error_message_count;
int error_one_per_line;
void (*error_print_progname) (void);

static const char *last_file;
static unsigned int last_line;

static void
print_prefix (const char *file, unsigned int line)
{
  fflush (stdout);

  if (error_print_progname != NULL)
    error_print_progname ();
  else
    fputs (program_invocation_name != NULL ? program_invocation_name : "",
           stderr);

  if (file != NULL)
    fprintf (stderr, ":%s:%u", file, line);

  fputs (": ", stderr);
}

static void
print_message (const char *file, unsigned int line, int errnum,
               const char *format, va_list ap)
{
  print_prefix (file, line);
  vfprintf (stderr, format, ap);
  if (errnum != 0)
    fprintf (stderr, ": %s", strerror (errnum));
  fputc ('\n', stderr);
  fflush (stderr);
  error_message_count++;
}

void
error (int status, int errnum, const char *format, ...)
{
  va_list ap;

  va_start (ap, format);
  print_message (NULL, 0, errnum, format, ap);
  va_end (ap);

  if (status != 0)
    exit (status);
}

void
error_at_line (int status, int errnum, const char *file_name,
               unsigned int line_number, const char *format, ...)
{
  va_list ap;

  if (error_one_per_line
      && last_file != NULL && last_line == line_number
      && strcmp (last_file, file_name) == 0)
    return;

  last_file = file_name;
  last_line = line_number;

  va_start (ap, format);
  print_message (file_name, line_number, errnum, format, ap);
  va_end (ap);

  if (status != 0)
    exit (status);
}
