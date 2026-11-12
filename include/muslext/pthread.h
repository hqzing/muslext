/* Minimal pthread extensions provided by muslext. */

#ifndef MUSLEXT_PTHREAD_H
#define MUSLEXT_PTHREAD_H 1

#include <pthread.h>
#include <sched.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifdef CPU_SETSIZE
int pthread_setaffinity_np (pthread_t thread, size_t cpusetsize,
                            const cpu_set_t *cpuset);
int pthread_getaffinity_np (pthread_t thread, size_t cpusetsize,
                            cpu_set_t *cpuset);
#endif

int pthread_tryjoin_np (pthread_t thread, void **retval);
int pthread_timedjoin_np (pthread_t thread, void **retval,
                          const struct timespec *abstime);
int pthread_yield (void);

#ifdef __cplusplus
}
#endif

#endif /* MUSLEXT_PTHREAD_H */
