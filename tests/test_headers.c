/* The minimal headers must be usable on their own. */

#include <muslext/getpass.h>
#include <muslext/qsort_r.h>
#include <muslext/shadow.h>
#include <muslext/statx.h>
#include <muslext/utmpx.h>

int
main (void)
{
  void *symbols[] = {
    (void *) getpass,
    (void *) qsort_r,
    (void *) getspnam,
    (void *) getspent,
    (void *) setspent,
    (void *) endspent,
    (void *) statx,
    (void *) getutxent,
    (void *) setutxent,
    (void *) endutxent,
    (void *) getutxid,
    (void *) getutxline,
    (void *) pututxline,
    (void *) utmpxname,
    (void *) updwtmpx,
  };

  return symbols[0] == 0;
}
