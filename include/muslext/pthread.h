/* Minimal pthread extensions provided by muslext. */

#ifndef MUSLEXT_PTHREAD_H
#define MUSLEXT_PTHREAD_H 1

#include <pthread.h>

#ifdef __cplusplus
extern "C" {
#endif

int pthread_tryjoin_np (pthread_t thread, void **retval);
int pthread_timedjoin_np (pthread_t thread, void **retval,
                          const struct timespec *abstime);
int pthread_yield (void);

#ifdef __cplusplus
}
#endif

#endif /* MUSLEXT_PTHREAD_H */
