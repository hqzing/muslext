/* glob_pattern_p: does a string contain glob metacharacters? */

#include <muslext/muslext.h>

int
glob_pattern_p (const char *pattern, int quote)
{
  int bracket = 0;

  while (*pattern != '\0')
    {
      if (*pattern == '\\' && quote)
        {
          pattern++;
          if (*pattern == '\0')
            break;
        }
      else if (*pattern == '[')
        bracket = 1;
      else if (*pattern == ']' && bracket)
        bracket = 0;
      else if (*pattern == '?' || *pattern == '*')
        return 1;
      pattern++;
    }
  return bracket;
}
