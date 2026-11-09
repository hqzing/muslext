/* pidfd interfaces. */

#ifndef MUSLEXT_SYS_PIDFD_H
#define MUSLEXT_SYS_PIDFD_H 1

#include <sys/types.h>

#ifdef __cplusplus
extern "C" {
#endif

int pidfd_open (pid_t pid, unsigned int flags);
int pidfd_getfd (int pidfd, int targetfd, unsigned int flags);

#ifdef __cplusplus
}
#endif

#endif
