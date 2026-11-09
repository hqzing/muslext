/* Minimal shadow password declarations provided by muslext. */

#ifndef MUSLEXT_SHADOW_H
#define MUSLEXT_SHADOW_H 1

#include <shadow.h>

#ifdef __cplusplus
extern "C" {
#endif

struct spwd *getspnam (const char *name);
struct spwd *getspent (void);
void setspent (void);
void endspent (void);

#ifdef __cplusplus
}
#endif

#endif /* MUSLEXT_SHADOW_H */
