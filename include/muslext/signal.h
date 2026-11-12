/* Minimal signal extensions provided by muslext. */

#ifndef MUSLEXT_SIGNAL_H
#define MUSLEXT_SIGNAL_H 1

#include <signal.h>

#ifdef __cplusplus
extern "C" {
#endif

int sigblock (int mask);
int sigsetmask (int mask);
const char *sigabbrev_np (int signal);
const char *sigdescr_np (int signal);

#ifdef __cplusplus
}
#endif

#endif /* MUSLEXT_SIGNAL_H */
