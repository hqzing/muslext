/* Minimal wchar extensions provided by muslext. */

#ifndef MUSLEXT_WCHAR_H
#define MUSLEXT_WCHAR_H 1

#include <locale.h>
#include <stddef.h>
#include <wchar.h>

#ifdef __cplusplus
extern "C" {
#endif

size_t wcslcpy (wchar_t *dest, const wchar_t *src, size_t size);
size_t wcslcat (wchar_t *dest, const wchar_t *src, size_t size);
long wcstol_l (const wchar_t *nptr, wchar_t **endptr, int base, locale_t locale);
unsigned long wcstoul_l (const wchar_t *nptr, wchar_t **endptr, int base,
                         locale_t locale);

#ifdef __cplusplus
}
#endif

#endif /* MUSLEXT_WCHAR_H */
