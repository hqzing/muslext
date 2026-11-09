/* Assorted GNU/glibc compatibility functions. */

#define _GNU_SOURCE 1

#include <ctype.h>
#include <errno.h>
#include <math.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>
#include <locale.h>
#include <complex.h>

#include <muslext/muslext.h>
#include <printf.h>

void *
rawmemchr (const void *s, int c)
{
  const unsigned char *p = s;

  while (*p != (unsigned char) c)
    p++;
  return (void *) p;
}

size_t
wcslcpy (wchar_t *dest, const wchar_t *src, size_t size)
{
  size_t length = 0;

  while (src[length] != L'\0')
    {
      if (length + 1 < size)
        dest[length] = src[length];
      length++;
    }
  if (size != 0)
    dest[length < size ? length : size - 1] = L'\0';
  return length;
}

size_t
wcslcat (wchar_t *dest, const wchar_t *src, size_t size)
{
  size_t dest_length = 0;
  size_t i = 0;

  while (dest_length < size && dest[dest_length] != L'\0')
    dest_length++;
  while (src[i] != L'\0')
    {
      if (dest_length + 1 < size)
        dest[dest_length] = src[i];
      dest_length++;
      i++;
    }
  if (size != 0)
    dest[dest_length < size ? dest_length : size - 1] = L'\0';
  return dest_length;
}

double
gamma (double x)
{
  return lgamma (x);
}

double
roundeven (double x)
{
  double integral;
  double fraction;

  if (!isfinite (x))
    return x + x;
  fraction = modf (x, &integral);
  if (fabs (fraction) < 0.5)
    return integral;
  if (fabs (fraction) > 0.5)
    return integral + (x > 0 ? 1 : -1);
  return fmod (integral, 2.0) == 0 ? integral : integral + (x > 0 ? 1 : -1);
}

float
roundevenf (float x)
{
  return (float) roundeven ((double) x);
}

double complex
clog10 (double complex z)
{
  return log10 (cabs (z)) + I * carg (z);
}

char *
qecvt (long double value, int ndigit, int *decpt, int *sign)
{
  static _Thread_local char buffer[128];
  char temp[256];
  char digits[256];
  char *exponent;
  int i;
  int out = 0;

  if (ndigit < 0)
    ndigit = 0;
  if (ndigit > 100)
    ndigit = 100;
  snprintf (temp, sizeof (temp), "%.*Le", ndigit > 0 ? ndigit - 1 : 0, value);
  *sign = temp[0] == '-' ? 1 : 0;
  exponent = strchr (temp, 'e');
  if (exponent == NULL)
    exponent = strchr (temp, 'E');
  *decpt = exponent != NULL ? atoi (exponent + 1) + 1 : 1;
  for (i = 0; temp[i] != '\0' && temp[i] != 'e' && temp[i] != 'E'; i++)
    if (temp[i] >= '0' && temp[i] <= '9')
      digits[out++] = temp[i];
  digits[out] = '\0';
  if (ndigit == 0)
    digits[0] = '\0', digits[1] = '\0';
  else
    {
      while (out < ndigit)
        digits[out++] = '0';
      digits[ndigit] = '\0';
    }
  memcpy (buffer, digits, (size_t) ndigit + 1);
  return buffer;
}

char *
qfcvt (long double value, int ndigit, int *decpt, int *sign)
{
  static _Thread_local char buffer[128];
  char temp[256];
  char digits[256];
  int i;
  int out = 0;
  int point;

  if (ndigit < 0)
    ndigit = 0;
  if (ndigit > 100)
    ndigit = 100;
  snprintf (temp, sizeof (temp), "%.*Lf", ndigit, value);
  *sign = temp[0] == '-' ? 1 : 0;
  point = 0;
  for (i = 0; temp[i] != '\0'; i++)
    {
      if (temp[i] == '.')
        point = out;
      else if (temp[i] >= '0' && temp[i] <= '9')
        digits[out++] = temp[i];
    }
  digits[out] = '\0';
  while (out < ndigit)
    digits[out++] = '0';
  digits[ndigit] = '\0';
  *decpt = point;
  memcpy (buffer, digits, (size_t) ndigit + 1);
  return buffer;
}

char *
qgcvt (long double value, int ndigit, char *buffer)
{
  snprintf (buffer, (size_t) ndigit + 32, "%.*Lg", ndigit, value);
  return buffer;
}

size_t
parse_printf_format (const char *fmt, size_t n, int *argtypes)
{
  size_t count = 0;
  size_t filled = 0;

  while (*fmt != '\0')
    {
      const char *start;
      int type = -1;
      int flags = 0;

      if (*fmt++ != '%')
        continue;
      if (*fmt == '%')
        {
          fmt++;
          continue;
        }

      start = fmt;
      while (*fmt != '\0' && strchr ("#0- +'I", *fmt) != NULL)
        fmt++;
      if (*fmt == '*')
        {
          if (filled < n)
            argtypes[filled] = PA_INT;
          filled++;
          count++;
          fmt++;
        }
      else
        while (isdigit ((unsigned char) *fmt))
          fmt++;
      if (*fmt == '.')
        {
          fmt++;
          if (*fmt == '*')
            {
              if (filled < n)
                argtypes[filled] = PA_INT;
              filled++;
              count++;
              fmt++;
            }
          else
            while (isdigit ((unsigned char) *fmt))
              fmt++;
        }

      if (fmt[0] == 'h' && fmt[1] == 'h')
        flags |= PA_FLAG_SHORT, fmt += 2;
      else if (fmt[0] == 'h')
        flags |= PA_FLAG_SHORT, fmt++;
      else if (fmt[0] == 'l' && fmt[1] == 'l')
        flags |= PA_FLAG_LONG_LONG, fmt += 2;
      else if (fmt[0] == 'l')
        flags |= PA_FLAG_LONG, fmt++;
      else if (fmt[0] == 'L')
        flags |= PA_FLAG_LONG_DOUBLE, fmt++;
      else if (fmt[0] == 'j' || fmt[0] == 'z' || fmt[0] == 't')
        flags |= PA_FLAG_LONG_LONG, fmt++;

      switch (*fmt)
        {
        case 'd': case 'i': case 'u': case 'o': case 'x': case 'X':
          type = PA_INT;
          break;
        case 'c':
          type = (flags & PA_FLAG_LONG) != 0 ? PA_WCHAR : PA_CHAR;
          break;
        case 's':
          type = (flags & PA_FLAG_LONG) != 0 ? PA_WSTRING : PA_STRING;
          break;
        case 'p':
          type = PA_POINTER;
          break;
        case 'f': case 'F': case 'e': case 'E': case 'g': case 'G':
        case 'a': case 'A':
          type = PA_DOUBLE;
          break;
        case 'n':
          type = PA_POINTER | PA_FLAG_PTR;
          break;
        default:
          type = PA_LAST;
          break;
        }

      if (type != -1)
        {
          if (filled < n)
            argtypes[filled] = type | flags;
          filled++;
          count++;
        }
      if (*fmt != '\0')
        fmt++;
      (void) start;
    }
  return count;
}
