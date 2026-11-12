/* GNU pthread affinity declarations must be usable when _GNU_SOURCE is set. */

#include <muslext/pthread_affinity.h>

int
main (void)
{
  cpu_set_t set;
  CPU_ZERO (&set);
  (void) pthread_getaffinity_np (pthread_self (), sizeof (set), &set);
  return 0;
}
