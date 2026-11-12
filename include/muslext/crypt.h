/* Minimal crypt extensions provided by muslext. */

#ifndef MUSLEXT_CRYPT_H
#define MUSLEXT_CRYPT_H 1

#include <crypt.h>

#ifdef __cplusplus
extern "C" {
#endif

char *crypt_r (const char *key, const char *salt, struct crypt_data *data);
char *crypt (const char *key, const char *salt);

#ifdef __cplusplus
}
#endif

#endif /* MUSLEXT_CRYPT_H */
