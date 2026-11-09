/* GNU dev_t encoding helpers. */

#include <sys/types.h>

#include <muslext/muslext.h>

unsigned int
gnu_dev_major (dev_t dev)
{
  return (unsigned int) (((dev >> 8) & 0xfff) | ((dev >> 32) & ~0xfffULL));
}

unsigned int
gnu_dev_minor (dev_t dev)
{
  return (unsigned int) ((dev & 0xff) | ((dev >> 12) & ~0xffULL));
}

dev_t
gnu_dev_makedev (unsigned int major, unsigned int minor)
{
  return (dev_t) (((unsigned long long) (major & 0xfff) << 8)
         | (minor & 0xff)
         | ((unsigned long long) (major & ~0xfffU) << 32)
         | ((unsigned long long) (minor & ~0xffU) << 12));
}
