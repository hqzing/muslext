/* Minimal sched extensions provided by muslext. */

#ifndef MUSLEXT_SCHED_H
#define MUSLEXT_SCHED_H 1

#include <sched.h>

#ifdef __cplusplus
extern "C" {
#endif

int getcpu (unsigned int *cpu, unsigned int *node);

#ifdef __cplusplus
}
#endif

#endif /* MUSLEXT_SCHED_H */
