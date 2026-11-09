#include <assert.h>
#include <string.h>
#include <unistd.h>
#include <muslext/muslext.h>

int
main (void)
{
  int pipefd[2];
  char *password;

  if (isatty (STDIN_FILENO))
    return 0; /* would read the terminal, not the pipe */

  assert (pipe (pipefd) == 0);
  assert (write (pipefd[1], "secret\n", 7) == 7);
  close (pipefd[1]);
  assert (dup2 (pipefd[0], STDIN_FILENO) == STDIN_FILENO);
  close (pipefd[0]);

  password = getpass ("");
  assert (password != NULL);
  assert (strcmp (password, "secret") == 0);
  return 0;
}
