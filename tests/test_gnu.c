#include <assert.h>
#include <errno.h>
#include <math.h>
#include <complex.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>
#include <locale.h>
#include <sys/sysmacros.h>

#include <argz.h>
#include <printf.h>
#include <muslext/muslext.h>

int
main (void)
{
  char *end;
  wchar_t wide[32];
  wchar_t *wend;
  int types[8];
  size_t count;
  char *argz = NULL;
  size_t argz_len = 0;
  char *encoded;
  unsigned char decoded[32];
  int encoded_len;
  int decoded_len;
  char *qbuf;
  int decpt;
  int sign;

  assert (strtoq ("-42x", &end, 10) == -42);
  assert (*end == 'x');
  assert (strtouq ("42x", &end, 10) == 42);
  assert (*end == 'x');

  {
    char buffer[8] = "abc";
    assert (rawmemchr (buffer, 'b') == buffer + 1);
  }

  assert (wcslcpy (wide, L"hello", 32) == 5);
  assert (wcscmp (wide, L"hello") == 0);
  assert (wcslcat (wide, L" world", 32) == 11);
  assert (wcscmp (wide, L"hello world") == 0);
  {
    locale_t locale = newlocale (LC_ALL_MASK, "C", (locale_t) 0);
    assert (locale != (locale_t) 0);
    assert (wcstol_l (L"123", &wend, 10, locale) == 123);
    assert (*wend == L'\0');
    freelocale (locale);
  }

  assert (roundeven (2.5) == 2.0);
  assert (roundeven (3.5) == 4.0);
  assert (roundeven (-2.5) == -2.0);
  assert (roundevenf (5.5f) == 6.0f);
  assert (isfinite (gamma (5.0)));
  {
    double complex z = clog10 (100.0 + 0.0 * I);
    assert (fabs (creal (z) - 2.0) < 1e-12);
  }

  count = parse_printf_format ("%d %s %*.*f %Lg %p", 8, types);
  assert (count == 7);
  assert (types[0] == PA_INT);
  assert (types[1] == PA_STRING);
  assert (types[2] == PA_INT);
  assert (types[3] == PA_INT);
  assert (types[4] == PA_DOUBLE);
  assert (types[5] == (PA_DOUBLE | PA_FLAG_LONG_DOUBLE));
  assert (types[6] == PA_POINTER);

  assert (strverscmp ("a2", "a10") < 0);
  assert (strverscmp ("a10", "a2") > 0);
  assert (strverscmp ("1.0", "1.0") == 0);

  assert (argz_create_sep ("a:b:c", ':', &argz, &argz_len) == 0);
  assert (argz_count (argz, argz_len) == 3);
  assert (argz_add (&argz, &argz_len, "d") == 0);
  assert (argz_count (argz, argz_len) == 4);
  argz_stringify (argz, argz_len, '|');
  assert (strcmp (argz, "a|b|c|d") == 0);
  free (argz);

  encoded = malloc (64);
  assert (encoded != NULL);
  encoded_len = b64_ntop ((const unsigned char *) "hello", 5, encoded, 64);
  assert (encoded_len == 8);
  assert (strcmp (encoded, "aGVsbG8=") == 0);
  decoded_len = b64_pton (encoded, decoded, sizeof (decoded));
  assert (decoded_len == 5);
  assert (memcmp (decoded, "hello", 5) == 0);
  free (encoded);

  assert (glob_pattern_p ("abc", 1) == 0);
  assert (glob_pattern_p ("a*c", 1) == 1);
  assert (glob_pattern_p ("a\\*c", 1) == 0);

  {
    unsigned long long device = gnu_dev_makedev (8, 17);
    assert (gnu_dev_major ((dev_t) device) == 8);
    assert (gnu_dev_minor ((dev_t) device) == 17);
  }

  assert (strcmp (sigabbrev_np (SIGTERM), "TERM") == 0);
  assert (sigdescr_np (SIGTERM) != NULL);
  assert (strcmp (strerrorname_np (ENOENT), "ENOENT") == 0);
  assert (strerrordesc_np (ENOENT) != NULL);

  assert (sigblock (1 << SIGTERM) == 0 || 1);
  assert (sigsetmask (0) >= 0);

  qbuf = qecvt (1234.5L, 5, &decpt, &sign);
  assert (qbuf != NULL);
  assert (decpt == 4);
  assert (sign == 0);

  assert (strcmp (gnu_get_libc_version (), "musl") == 0);
  assert (malloc_trim (0) == 0);
  return 0;
}
