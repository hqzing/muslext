/* Minimal resolver extensions provided by muslext. */

#ifndef MUSLEXT_RESOLV_H
#define MUSLEXT_RESOLV_H 1

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

int b64_ntop (const unsigned char *src, size_t srclen, char *target,
              size_t targsize);
int b64_pton (const char *src, unsigned char *target, size_t targsize);

#ifdef __cplusplus
}
#endif

#endif /* MUSLEXT_RESOLV_H */
