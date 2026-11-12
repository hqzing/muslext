/* Minimal stdlib extensions provided by muslext. */

#ifndef MUSLEXT_STDLIB_H
#define MUSLEXT_STDLIB_H 1

#include <stddef.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

void *reallocarray (void *ptr, size_t nmemb, size_t size);
char *canonicalize_file_name (const char *name);
long long strtoq (const char *nptr, char **endptr, int base);
unsigned long long strtouq (const char *nptr, char **endptr, int base);
char *qecvt (long double value, int ndigit, int *decpt, int *sign);
char *qfcvt (long double value, int ndigit, int *decpt, int *sign);
char *qgcvt (long double value, int ndigit, char *buffer);

#ifdef __cplusplus
}
#endif

#endif /* MUSLEXT_STDLIB_H */
