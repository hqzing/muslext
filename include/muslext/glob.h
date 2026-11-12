/* Minimal glob extensions provided by muslext. */

#ifndef MUSLEXT_GLOB_H
#define MUSLEXT_GLOB_H 1

#include <glob.h>

#ifdef __cplusplus
extern "C" {
#endif

int glob_pattern_p (const char *pattern, int quote);

#ifdef __cplusplus
}
#endif

#endif /* MUSLEXT_GLOB_H */
