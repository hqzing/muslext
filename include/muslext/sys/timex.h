/* Minimal timex extensions provided by muslext. */

#ifndef MUSLEXT_SYS_TIMEX_H
#define MUSLEXT_SYS_TIMEX_H 1

#include <sys/timex.h>

#ifdef __cplusplus
extern "C" {
#endif

int ntp_adjtime (void *buf);

#ifdef __cplusplus
}
#endif

#endif /* MUSLEXT_SYS_TIMEX_H */
