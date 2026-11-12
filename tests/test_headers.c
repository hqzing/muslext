/* The minimal headers must be usable on their own. */

#include <muslext/getpass.h>
#include <muslext/qsort_r.h>
#include <muslext/shadow.h>
#include <muslext/statx.h>
#include <muslext/utmpx.h>
#include <muslext/crypt.h>
#include <muslext/fpu_control.h>
#include <muslext/glob.h>
#include <muslext/gnu-libc-version.h>
#include <muslext/malloc.h>
#include <muslext/math.h>
#include <muslext/pthread.h>
#include <muslext/resolv.h>
#include <muslext/sched.h>
#include <muslext/setjmp.h>
#include <muslext/signal.h>
#include <muslext/stdlib.h>
#include <muslext/string.h>
#include <muslext/sys/mount.h>
#include <muslext/sys/sysmacros.h>
#include <muslext/sys/timex.h>
#include <muslext/sys/uio.h>
#include <muslext/unistd.h>
#include <muslext/wchar.h>

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
