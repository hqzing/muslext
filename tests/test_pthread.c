/* Non-GNU and GNU-dependent pthread extensions are split. */

#include <muslext/pthread.h>

int
main (void)
{
  (void) pthread_tryjoin_np;
  (void) pthread_timedjoin_np;
  (void) pthread_yield;
  return 0;
}
