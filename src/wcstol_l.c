/* Locale-aware wide integer conversions; the locale argument is ignored. */

#undef _GNU_SOURCE
#include <locale.h>
#include <wchar.h>

long
wcstol_l (const wchar_t *nptr, wchar_t **endptr, int base, locale_t locale)
{
  (void) locale;
  return wcstol (nptr, endptr, base);
}

unsigned long
wcstoul_l (const wchar_t *nptr, wchar_t **endptr, int base, locale_t locale)
{
  (void) locale;
  return wcstoul (nptr, endptr, base);
}
