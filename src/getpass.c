/* getpass(3): read a password from the controlling terminal. */

#define _GNU_SOURCE 1

#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>

#include <muslext/muslext.h>

char *
getpass (const char *prompt)
{
  static char buffer[128];
  struct termios old_state;
  struct termios new_state;
  int fd;
  int restore = 0;
  ssize_t length;

  fd = open ("/dev/tty", O_RDWR | O_NOCTTY);
  if (fd < 0)
    fd = STDIN_FILENO;

  if (prompt != NULL)
    {
      size_t prompt_length = strlen (prompt);

      if (write (STDERR_FILENO, prompt, prompt_length) < 0)
        {
          /* Best effort: the prompt is not essential. */
        }
    }

  if (tcgetattr (fd, &old_state) == 0)
    {
      new_state = old_state;
      new_state.c_lflag &= ~(ECHO);
      if (tcsetattr (fd, TCSAFLUSH, &new_state) == 0)
        restore = 1;
    }

  length = read (fd, buffer, sizeof (buffer) - 1);
  if (restore)
    tcsetattr (fd, TCSAFLUSH, &old_state);
  if (fd != STDIN_FILENO)
    close (fd);

  if (length < 0)
    length = 0;
  buffer[length] = '\0';
  if (length > 0 && buffer[length - 1] == '\n')
    buffer[length - 1] = '\0';
  return buffer;
}
