/* shadow database delegation. */

#include <shadow.h>

#include "internal.h"

struct spwd *
getspnam (const char *name)
{
  static struct spwd *(*real) (const char *);

  if (real == NULL)
    real = (struct spwd *(*)(const char *)) muslext_resolve ("getspnam",
                                                             (void *) getspnam);
  return real != NULL ? real (name) : NULL;
}

struct spwd *
getspent (void)
{
  static struct spwd *(*real) (void);

  if (real == NULL)
    real = (struct spwd *(*)(void)) muslext_resolve ("getspent",
                                                     (void *) getspent);
  return real != NULL ? real () : NULL;
}

void
setspent (void)
{
  static void (*real) (void);

  if (real == NULL)
    real = (void (*) (void)) muslext_resolve ("setspent", (void *) setspent);
  if (real != NULL)
    real ();
}

void
endspent (void)
{
  static void (*real) (void);

  if (real == NULL)
    real = (void (*) (void)) muslext_resolve ("endspent", (void *) endspent);
  if (real != NULL)
    real ();
}
