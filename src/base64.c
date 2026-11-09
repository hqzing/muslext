/* Base64 encoding and decoding. */

#include <stddef.h>
#include <string.h>

#include <muslext/muslext.h>

static const char alphabet[] =
  "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

int
b64_ntop (const unsigned char *src, size_t srclen, char *target,
          size_t targsize)
{
  size_t out = 0;
  size_t i;

  for (i = 0; i < srclen; i += 3)
    {
      unsigned int value = (unsigned int) src[i] << 16;
      size_t remaining = srclen - i;

      if (remaining > 1)
        value |= (unsigned int) src[i + 1] << 8;
      if (remaining > 2)
        value |= src[i + 2];

      if (out + 4 >= targsize)
        return -1;
      target[out++] = alphabet[(value >> 18) & 0x3f];
      target[out++] = alphabet[(value >> 12) & 0x3f];
      target[out++] = remaining > 1 ? alphabet[(value >> 6) & 0x3f] : '=';
      target[out++] = remaining > 2 ? alphabet[value & 0x3f] : '=';
    }
  if (out >= targsize)
    return -1;
  target[out] = '\0';
  return (int) out;
}

static int
decode_char (unsigned char c)
{
  if (c >= 'A' && c <= 'Z')
    return c - 'A';
  if (c >= 'a' && c <= 'z')
    return c - 'a' + 26;
  if (c >= '0' && c <= '9')
    return c - '0' + 52;
  if (c == '+')
    return 62;
  if (c == '/')
    return 63;
  return -1;
}

int
b64_pton (const char *src, unsigned char *target, size_t targsize)
{
  unsigned int value = 0;
  int bits = 0;
  size_t out = 0;
  int padding = 0;

  while (*src != '\0')
    {
      int digit;

      if (*src == '=')
        {
          padding++;
          src++;
          continue;
        }
      if (*src == ' ' || *src == '\t' || *src == '\r' || *src == '\n')
        {
          src++;
          continue;
        }
      digit = decode_char ((unsigned char) *src++);
      if (digit < 0)
        return -1;
      if (padding != 0)
        return -1;
      value = (value << 6) | (unsigned int) digit;
      bits += 6;
      if (bits >= 8)
        {
          bits -= 8;
          if (out >= targsize)
            return -1;
          target[out++] = (unsigned char) (value >> bits);
        }
    }
  if (padding > 2 || bits >= 6)
    return -1;
  return (int) out;
}
