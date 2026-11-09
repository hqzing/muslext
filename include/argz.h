/* GNU argz vectors for systems without argz.h. */

#ifndef MUSLEXT_ARGZ_H
#define MUSLEXT_ARGZ_H 1

#include <stddef.h>
#include <errno.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef int error_t;

error_t argz_create (char *const argv[], char **argz, size_t *argz_len);
error_t argz_create_sep (const char *string, int sep, char **argz,
                         size_t *argz_len);
size_t argz_count (const char *argz, size_t argz_len);
void argz_stringify (char *argz, size_t argz_len, int sep);
error_t argz_add (char **argz, size_t *argz_len, const char *str);
error_t argz_append (char **argz, size_t *argz_len, const char *buf,
                     size_t buf_len);
error_t argz_insert (char **argz, size_t *argz_len, char *before,
                     const char *entry);
error_t argz_add_sep (char **argz, size_t *argz_len, const char *string,
                      int delim);

#ifdef __cplusplus
}
#endif

#endif
