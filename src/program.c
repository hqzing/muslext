/* program_invocation_name / program_invocation_short_name. */

#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

char *program_invocation_name = NULL;
char *program_invocation_short_name = NULL;

static void
set_names (char *name)
{
  char *slash;

  program_invocation_name = name;
  slash = strrchr (name, '/');
  program_invocation_short_name = slash != NULL ? slash + 1 : name;
}

__attribute__ ((constructor)) static void
muslext_init_program_name (void)
{
  char buffer[4096];
  ssize_t length;
  int fd;

  if (program_invocation_name != NULL)
    return;

  fd = open ("/proc/self/cmdline", O_RDONLY | O_CLOEXEC);
  if (fd < 0)
    {
      set_names (strdup (""));
      return;
    }

  length = read (fd, buffer, sizeof (buffer) - 1);
  close (fd);
  if (length <= 0)
    {
      set_names (strdup (""));
      return;
    }

  buffer[length] = '\0';
  set_names (strdup (buffer));
}
