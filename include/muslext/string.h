/* Minimal string extensions provided by muslext. */

#ifndef MUSLEXT_STRING_H
#define MUSLEXT_STRING_H 1

#include <stddef.h>
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

int __xpg_strerror_r (int errnum, char *buf, size_t buflen);
void *rawmemchr (const void *s, int c);
int strverscmp (const char *left, const char *right);
const char *strerrorname_np (int error);
const char *strerrordesc_np (int error);

#ifdef __cplusplus
}
#endif

#endif /* MUSLEXT_STRING_H */
