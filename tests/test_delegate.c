#include <assert.h>
#include <errno.h>
#include <string.h>

#include <muslext/muslext.h>

int
main (void)
{
  char buffer[128];
  struct utmpx *entry;
  struct spwd *shadow;

  setutxent ();
  while ((entry = getutxent ()) != NULL)
    (void) entry;
  endutxent ();
  (void) utmpxname ("/nonexistent");

  shadow = getspnam ("root");
  (void) shadow;
  setspent ();
  (void) getspent ();
  endspent ();

  assert (__xpg_strerror_r (ENOENT, buffer, sizeof (buffer)) == 0);
  assert (strlen (buffer) != 0);

  assert (program_invocation_name != NULL);
  assert (program_invocation_short_name != NULL);
  return 0;
}
