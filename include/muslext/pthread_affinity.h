/* Minimal pthread affinity extensions provided by muslext. */

#ifndef MUSLEXT_PTHREAD_AFFINITY_H
#define MUSLEXT_PTHREAD_AFFINITY_H 1

#include <pthread.h>
#include <sched.h>

#ifndef CPU_SETSIZE
# error "muslext/pthread_affinity.h requires _GNU_SOURCE before any system header"
#endif

#ifdef __cplusplus
extern "C" {
#endif

int pthread_setaffinity_np (pthread_t thread, size_t cpusetsize,
                            const cpu_set_t *cpuset);
int pthread_getaffinity_np (pthread_t thread, size_t cpusetsize,
                            cpu_set_t *cpuset);

#ifdef __cplusplus
}
#endif

#endif /* MUSLEXT_PTHREAD_AFFINITY_H */
