/* glibc printf.h subset for systems without it. */

#ifndef MUSLEXT_PRINTF_H
#define MUSLEXT_PRINTF_H 1

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

#define PA_INT 0
#define PA_CHAR 1
#define PA_WCHAR 2
#define PA_STRING 3
#define PA_WSTRING 4
#define PA_POINTER 5
#define PA_FLOAT 6
#define PA_DOUBLE 7
#define PA_LAST 8

#define PA_FLAG_MASK 0xff00
#define PA_FLAG_LONG_LONG (1 << 8)
#define PA_FLAG_LONG_DOUBLE PA_FLAG_LONG_LONG
#define PA_FLAG_LONG (1 << 9)
#define PA_FLAG_SHORT (1 << 10)
#define PA_FLAG_PTR (1 << 11)

size_t parse_printf_format (const char *fmt, size_t n, int *argtypes);

#ifdef __cplusplus
}
#endif

#endif
