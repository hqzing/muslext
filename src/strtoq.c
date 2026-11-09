/* BSD strtoq/strtouq aliases.
   Kept in a translation unit that does not request _GNU_SOURCE, because
   glibc's C23 header redirects strtoll/strtoull (and strtoq itself) to
   __isoc23_* symbols under _GNU_SOURCE. */

#undef _GNU_SOURCE
#include <stdlib.h>

long long
strtoq (const char *nptr, char **endptr, int base)
{
  return strtoll (nptr, endptr, base);
}

unsigned long long
strtouq (const char *nptr, char **endptr, int base)
{
  return strtoull (nptr, endptr, base);
}
