/* GNU error.h replacement for systems whose libc does not ship it. */

#ifndef MUSLEXT_ERROR_H
#define MUSLEXT_ERROR_H 1

#include <stdarg.h>
#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

extern char *program_invocation_name;

extern unsigned int error_message_count;
extern int error_one_per_line;
extern void (*error_print_progname) (void);

void error (int status, int errnum, const char *format, ...)
  __attribute__ ((__format__ (__printf__, 3, 4)));
void error_at_line (int status, int errnum, const char *file_name,
                    unsigned int line_number, const char *format, ...)
  __attribute__ ((__format__ (__printf__, 5, 6)));

#ifdef __cplusplus
}
#endif

#endif /* MUSLEXT_ERROR_H */
